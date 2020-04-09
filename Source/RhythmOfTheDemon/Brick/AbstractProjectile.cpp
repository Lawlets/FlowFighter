// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractProjectile.h"
#include "Actors/AbstractEntityActor.h"
#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "Actors/GameplaySpaceAnchor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"
#include "FMODBlueprintStatics.h"

// Sets default values
AAbstractProjectile::AAbstractProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	m_mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("mainMesh"));
	m_mesh->SetupAttachment(m_root);

	m_afterReflectionParticle = CreateOptionalDefaultSubobject<UParticleSystemComponent>(TEXT("afterReflectionParticle"));
	m_afterReflectionParticle->SetupAttachment(m_mesh);

	m_onMissParticle = CreateOptionalDefaultSubobject<UParticleSystemComponent>(TEXT("onMissParticle"));
	m_onMissParticle->SetupAttachment(m_root);

	m_onSwitchHitParticle = CreateOptionalDefaultSubobject<UParticleSystemComponent>(TEXT("onSwitchHitParticle"));
	m_onSwitchHitParticle->SetupAttachment(m_root);

	m_reflectFlipbookComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("ReflectFlipBook"));
	m_reflectFlipbookComponent->SetupAttachment(m_root);
}

// Called when the game starts or when spawned
void AAbstractProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_dynamicMaterial = UMaterialInstanceDynamic::Create(m_mesh->GetMaterial(0), this, FName("Mat Instance"));
	m_mesh->SetMaterial(0, m_dynamicMaterial);

	m_dissolveComponent->Init(m_dynamicMaterial, &GetWorldTimerManager());
	m_dissolveComponent->Hide();

	m_reflectFlipbookComponent->Stop();
	m_reflectFlipbookComponent->SetLooping(false);
}

void AAbstractProjectile::Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target)
{
	Super::Init(travelTime, caster, target);

	m_currPath = m_pathCurve;
	m_isReflected = false;
	m_lastTimePos = 0;

	m_onMissParticle->DeactivateSystem();
	m_onSwitchHitParticle->DeactivateSystem();
}

// Called every frame
void AAbstractProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (state == EBRICK_STATE::PASSED) // Forced proj to stay in position while being dissolved
	{
		AAbstractEntityActor* target = (m_isReflected ? m_enemy : m_player);

		SetActorLocation(target->GetHitPoint()->GetComponentLocation());
		SetActorRotation(target->GetHitPoint()->GetComponentRotation());
	}
}

void AAbstractProjectile::Update(int timelinePosition)
{
	Super::Update(timelinePosition);
	m_lastTimePos = timelinePosition;

	AAbstractEntityActor* caster = (m_isReflected ? m_player : m_enemy);
	AAbstractEntityActor* target = (m_isReflected ? m_enemy : m_player);

	FVector casterPos = caster->GetCastPoint()->GetComponentLocation();
	FVector targetPos = target->GetHitPoint()->GetComponentLocation();

	if (m_isReflected)
		casterPos += m_lockOffset;

	//Progress
	int timeSinceCreation = timelinePosition - m_creationTimelinePosition;
	float progression = (float)(timeSinceCreation) / (m_travelTime + activationWindow);

	//Location
	FVector newPos = ComputePosition(casterPos, targetPos, progression);
	SetActorLocation(newPos);

	//Rotation
	FVector nextPos = ComputePosition(casterPos, targetPos, progression + 0.01);

	FVector newForward = newPos - nextPos;
	SetActorRotation(newForward.Rotation());
}

FVector AAbstractProjectile::ComputePosition(FVector begin, FVector end, float progression)
{
	FVector newPos = FMath::Lerp(begin, end, progression);

	// Curve Offset
	float curveValue = m_currPath->GetFloatValue(progression);
	FVector rightOffset = curveValue * (m_player->GetActorRightVector() * m_curveDirection.X * m_curveIntensity);
	FVector UpOffset = curveValue * (m_player->GetActorUpVector() * m_curveDirection.Y * m_curveIntensity);

	return newPos + rightOffset + UpOffset;
}

EINTERACTION_RESULT AAbstractProjectile::Interact(float timelinePos, bool isFromArmPlayer)
{
	EINTERACTION_RESULT res = AAbstractBrick::Interact(timelinePos, isFromArmPlayer);

	if (res == EINTERACTION_RESULT::Valid)
		BroadcastBrickInteraction();

	m_interactionState = res;
	return res;
}

void AAbstractProjectile::SwitchState(EBRICK_STATE newState)
{
	Super::SwitchState(newState);

	switch (newState)
	{
	case EBRICK_STATE::IDLE:
		m_dissolveComponent->Ressolve(m_appearDuration);
		break;
		/*case EBRICK_STATE::PREPARATION:
			break;*/
	case EBRICK_STATE::HIT:
		if (IsInteractable())
			OnSwitchToHitState();
			break;
	case EBRICK_STATE::PASSED:


		if (m_interactionState != EINTERACTION_RESULT::Valid || m_isReflected) // If Valid at End , Auto reflect => Need Synchronisation with Anim
		{
			if (m_isReflected) //Damage 
			{
				FDamageEvent dmgEvent;
				m_enemy->TakeDamage(m_dynamicScoreValue, dmgEvent, nullptr, nullptr);
			}

			if (m_interactionState != EINTERACTION_RESULT::Valid && !m_isReflected)
				OnMissedFeedback();

			m_dissolveComponent->OnDissolveComplete().AddUObject(this, &AAbstractProjectile::Hide);
			m_dissolveComponent->Dissolve(m_disappearDuration);
		}
		//else // If valid && Not reflected
		//	Reflect();

		break;
	default:
		break;
	}
}

void AAbstractProjectile::Reflect()
{
	//Swap Value
	m_isReflected = true;
	m_currPath = m_reflectedPathCurve;
	m_lockOffset = GetActorLocation() - m_player->GetCastPoint()->GetComponentLocation();

	//Feedback
	OnReflectFeedback();

	//Reset Timeline and set new traveling
	m_creationTimelinePosition = m_lastTimePos;
	m_travelTime = m_reflectTravelDuration; // milisecond
	m_timelinePosition = m_creationTimelinePosition + m_travelTime;

	SwitchState(EBRICK_STATE::HIT);
}

void AAbstractProjectile::OnMissedFeedback()
{
	m_onMissParticle->AttachToComponent(m_gameplaySpaceAnchor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	m_onMissParticle->ActivateSystem();
}

void AAbstractProjectile::OnReflectFeedback()
{
	m_reflectFlipbookComponent->SetWorldLocation(GetActorLocation());
	m_reflectFlipbookComponent->SetWorldRotation(m_player->GetActorRightVector().Rotation());
	//m_successType
	FVector scale = m_reflectFlipbookComponent->GetComponentScale();
	//	m_reflectFlipbookComponent->SetFlipbook(m_reflectAnim); // Set flipbook Depending on success
	m_reflectFlipbookComponent->SetFlipbook(m_reflectAnims[(int)m_successType]); // Set flipbook Depending on success

	m_reflectFlipbookComponent->AttachToComponent(m_gameplaySpaceAnchor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	m_reflectFlipbookComponent->SetWorldScale3D(scale);
	m_reflectFlipbookComponent->PlayFromStart();
}

void AAbstractProjectile::OnSwitchToHitState()
{
	UE_LOG(ApplicationInitLog, Log, TEXT(" Switch to Hit Particle !"));
	m_onSwitchHitParticle->AttachToComponent(m_gameplaySpaceAnchor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	m_onSwitchHitParticle->ActivateSystem();
}

void AAbstractProjectile::SetSuccessType(ESUCCESS_TYPE success)
{
	Super::SetSuccessType(success);

	Reflect();
}

void AAbstractProjectile::Show()
{
	Super::Show();
	m_mesh->SetVisibility(true);
}

void AAbstractProjectile::Hide()
{
	Super::Hide();
	m_mesh->SetVisibility(false);
}