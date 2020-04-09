// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractDodgeableBrick.h"
#include "Components/SkeletalMeshComponent.h"

#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "FMODBlueprintStatics.h"

#include "Actors/GameplaySpaceAnchor.h"
#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "Actors/Rail.h"
#include "Actors/AbstractEntityActor.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

AAbstractDodgeableBrick::AAbstractDodgeableBrick(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	m_rightHand = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletalRightHand"));
	m_rightHand->SetupAttachment(m_root);

	m_leftHand = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletalLeftHand"));
	m_leftHand->SetupAttachment(m_root);

	m_clapFlipbook = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("clapFlipBook"));
	m_clapFlipbook->SetupAttachment(m_root);

	m_clapParticle = CreateOptionalDefaultSubobject<UParticleSystemComponent>(TEXT("clapParticle"));
	m_clapParticle->SetupAttachment(m_root);

	const ConstructorHelpers::FObjectFinder<UCurveFloat> DodgeableBrickCurve(TEXT("CurveFloat'/Game/Curve/DodgeableBrickCurve.DodgeableBrickCurve'"));
	m_pathCurve = DodgeableBrickCurve.Object;

	m_validDashFXComp = CreateOptionalDefaultSubobject<UParticleSystemComponent>(TEXT("validDashParticle"));
	m_validDashFXComp->SetupAttachment(m_root);
}

// Called when the game starts or when spawned
void AAbstractDodgeableBrick::BeginPlay()
{
	Super::BeginPlay();

	m_gameplaySpaceAnchor = AGameplaySpaceAnchor::GetInstance();

	//Set Up Dynamic Brick Material
	m_dynamicMaterial = UMaterialInstanceDynamic::Create(m_rightHand->GetMaterial(0), this, FName("Mat Instance"));
	m_rightHand->SetMaterial(0, m_dynamicMaterial);
	m_leftHand->SetMaterial(0, m_dynamicMaterial);

	m_dissolveComponent->Init(m_dynamicMaterial, &GetWorldTimerManager());
	m_dissolveComponent->Hide();

	m_clapFlipbook->SetLooping(false);
	m_clapFlipbook->Stop();

	UAnimInstance* animInstance = m_rightHand->GetAnimInstance();
	if (animInstance)
		m_animInstance = Cast<UBrickAnimInstance>(animInstance);

	animInstance = m_leftHand->GetAnimInstance();
	if (animInstance)
		m_secondHandAnimInstance = Cast<UBrickAnimInstance>(animInstance);
}

void AAbstractDodgeableBrick::Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target)
{
	Super::Init(travelTime, caster, target);

	m_clapParticle->DeactivateSystem();
	m_validDashFXComp->DeactivateSystem();
}

// Called every frame
void AAbstractDodgeableBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbstractDodgeableBrick::Update(int timelinePosition)
{
	Super::Update(timelinePosition);

	//Rotation
	FQuat orientation = m_player->GetActorRotation().Quaternion();
	SetActorRotation(orientation);

	//Obstacle Location
	if (state == EBRICK_STATE::IDLE)
	{
		int timeSinceCreation = timelinePosition - m_creationTimelinePosition;
		float progression = (float)(timeSinceCreation) / (m_travelTime - errorWindow);
		SetActorLocation(FMath::LerpStable(m_enemy->GetCastPoint()->GetComponentLocation(), m_player->GetHitPoint()->GetComponentLocation(), progression));
	}
	else if (state == EBRICK_STATE::PREPARATION)
	{
		SetActorLocation(m_player->GetHitPoint()->GetComponentLocation());
	}
	else
	{	// Lock Position
		SetActorLocation(m_gameplaySpaceAnchor->GetActorLocation() + m_lockPositionOffset);
	}

	//Hand Location
	FVector handDirection = (m_isMvmtHorizontal ? GetActorRightVector() : GetActorUpVector());

	ComputeHandLocation(timelinePosition, -handDirection, m_rightHand, m_rightHandSocket);
	ComputeHandLocation(timelinePosition, handDirection, m_leftHand, m_leftHandSocket);
}

EINTERACTION_RESULT AAbstractDodgeableBrick::Interact(float timelinePos, bool isFromArmPlayer)
{
	EINTERACTION_RESULT res = Super::Interact(timelinePos, isFromArmPlayer);

	if (res == EINTERACTION_RESULT::Valid)
		BroadcastBrickInteraction();

	m_interactionState = res;
	return res;
}

void AAbstractDodgeableBrick::ComputeHandLocation(int timelinePosition, FVector direction, USkeletalMeshComponent* handMesh, FName offsetSocketName)
{
	FVector distOffset = m_distOffsetFromPlayer * direction;

	if (state == EBRICK_STATE::IDLE)
	{
		//Progress
		int timeSinceCreation = timelinePosition - m_creationTimelinePosition;
		float progression = (float)(timeSinceCreation) / (m_travelTime - errorWindow);

		//UE_LOG(ApplicationInitLog, Log, TEXT("* Dodgeable progression : %f"), progression);

		// Curve Offset
		float curveValue = m_pathCurve->GetFloatValue(progression);
		FVector curveOffset = curveValue * (direction * m_curveIntensity);
		FVector newPos = GetActorLocation() + distOffset + curveOffset;

		handMesh->SetWorldLocation(newPos);
	}
	else if (state == EBRICK_STATE::PREPARATION)
	{
		FVector newPos = GetActorLocation() + distOffset;

		handMesh->SetWorldLocation(newPos);
	}
	else if (state == EBRICK_STATE::HIT)
	{
		int timeActivation = m_timelinePosition - activationWindow;
		int timeSinceActivation = timelinePosition - timeActivation;
		float progression = (float)(timeSinceActivation) / (activationWindow * 2); // window is before and after the timelinepos => *2

		FVector originPos = GetActorLocation() + distOffset;

		if (progression < m_waitBeforeClap)
			progression = 0.f;
		else
			timeSinceActivation = timelinePosition - (m_waitBeforeClap * 1000.f); // Milisec

		FVector newPos = FMath::Lerp(originPos, GetActorLocation(), progression);

		handMesh->SetWorldLocation(newPos);
	}
	else
	{
		handMesh->SetWorldLocation(GetActorLocation());
	}

}

void AAbstractDodgeableBrick::SwitchState(EBRICK_STATE newState)
{
	Super::SwitchState(newState);

	if (m_animInstance)
		m_animInstance->PlayAnimFromState(newState);

	if (m_secondHandAnimInstance)
		m_secondHandAnimInstance->PlayAnimFromState(newState);

	switch (newState)
	{
	case EBRICK_STATE::IDLE:
		m_dissolveComponent->Ressolve(m_appearDuration);
		break;
		/*case EBRICK_STATE::PREPARATION:
			break;*/
	case EBRICK_STATE::HIT:
		SetActorLocation(m_player->GetHitPoint()->GetComponentLocation()); //Ensure position is Okay before Locking
		LockPosition();
		break;
	case EBRICK_STATE::PASSED:

		//Feedback
		OnClapFeedback();

		m_dissolveComponent->OnDissolveComplete().AddUObject(this, &AAbstractDodgeableBrick::Hide);
		m_dissolveComponent->Dissolve(m_disappearDuration);
		break;
	default:
		break;
	}
}

void AAbstractDodgeableBrick::LockPosition()
{
	m_lockPositionOffset = GetActorLocation() - m_gameplaySpaceAnchor->GetActorLocation();
}

void AAbstractDodgeableBrick::OnClapFeedback()
{
	UFMODBlueprintStatics::PlayEvent2D(this, m_clapSFX, true);

	m_clapParticle->AttachToComponent(m_gameplaySpaceAnchor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	m_clapParticle->ActivateSystem();

	if (m_interactionState != EINTERACTION_RESULT::Valid)
		m_clapFlipbook->PlayFromStart();
}

void AAbstractDodgeableBrick::SetSuccessType(ESUCCESS_TYPE success)
{
	Super::SetSuccessType(success);

	m_validDashFXComp->SetTemplate(m_dashFXs[(int)success]); // Set flipbook Depending on success
	m_validDashFXComp->AttachToComponent(m_player->GetCastPoint(), FAttachmentTransformRules::KeepRelativeTransform);
	m_validDashFXComp->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	m_validDashFXComp->ActivateSystem();
}

void AAbstractDodgeableBrick::Show()
{
	Super::Show();
	m_rightHand->SetVisibility(true);
	m_leftHand->SetVisibility(true);
}

void AAbstractDodgeableBrick::Hide()
{
	Super::Hide();
	m_rightHand->SetVisibility(false);
	m_leftHand->SetVisibility(false);
}