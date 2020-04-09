// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopBrick.h"
#include "Actors/AbstractEntityActor.h"
#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "Actors/GameplaySpaceAnchor.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"
#include "FMODBlueprintStatics.h"

// Sets default values
ACoopBrick::ACoopBrick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	m_mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletalMesh"));
	m_mesh->SetupAttachment(m_root);

	m_shieldParticle = CreateOptionalDefaultSubobject<UParticleSystemComponent>(TEXT("ShieldParticle"));
	m_shieldParticle->SetupAttachment(m_root);
}

// Called when the game starts or when spawned
void ACoopBrick::BeginPlay()
{
	Super::BeginPlay();

	m_dynamicMaterial = UMaterialInstanceDynamic::Create(m_mesh->GetMaterial(0), this, FName("Mat Instance"));
	m_mesh->SetMaterial(0, m_dynamicMaterial);

	m_dissolveComponent->Init(m_dynamicMaterial, &GetWorldTimerManager());

	UAnimInstance* animInstance = m_mesh->GetAnimInstance();
	if (animInstance)
		m_animInstance = Cast<UBrickAnimInstance>(animInstance);
}

void ACoopBrick::Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target)
{
	Super::Init(travelTime, caster, target);

	m_armPlayerActivated = false;
	m_legPlayerActivated = false;

	m_shieldParticle->DeactivateSystem();
}

// Called every frame
void ACoopBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoopBrick::Update(int timelinePosition)
{
	Super::Update(timelinePosition);

	FQuat targetRot = m_player->GetActorRotation().Quaternion();
	SetActorRotation(targetRot);

	// Position
	FVector targetPos = m_player->GetHitPoint()->GetComponentLocation();
	FVector enemyLocation = m_enemy->GetCastPoint()->GetComponentLocation();

	if (state == EBRICK_STATE::IDLE)
	{
		SetActorLocation(FMath::Lerp(enemyLocation, targetPos, m_idlePositionRatio));
	}
	else if (state == EBRICK_STATE::PREPARATION)
	{
		SetActorLocation(FMath::Lerp(enemyLocation, targetPos, m_prepPositionRatio));
	}
	else
	{
		SetActorLocation(FMath::Lerp(enemyLocation, targetPos, m_hitPositionRatio));
	}
}

EINTERACTION_RESULT ACoopBrick::Interact(float timelinePos, bool isFromArmPlayer)
{
	EINTERACTION_RESULT res = AAbstractBrick::Interact(timelinePos, isFromArmPlayer);

	if (res != EINTERACTION_RESULT::Valid)
	{
		if (res == EINTERACTION_RESULT::Miss)
			m_interactionState = EINTERACTION_RESULT::Miss;
		
		return res;
	}

	if (isFromArmPlayer)
	{
		m_armPlayerActivated = true;
	}
	else
		m_legPlayerActivated = true;

	if (m_armPlayerActivated && m_legPlayerActivated)
	{
		m_enemy->SetVisible(true, m_dissolveDuration);
		BroadcastBrickInteraction();

		m_interactionState = EINTERACTION_RESULT::Valid;
		return EINTERACTION_RESULT::Valid;
	}

	return EINTERACTION_RESULT::None;
}

void ACoopBrick::SwitchState(EBRICK_STATE newState)
{
	Super::SwitchState(newState);

	if (m_animInstance)
		m_animInstance->PlayAnimFromState(newState);

	//PlayAnimationState((int)state);

	switch (newState)
	{
	case EBRICK_STATE::IDLE:
		m_enemy->SetVisible(false, m_dissolveDuration);
		Blink(GetIdleWindow() / 1000.f);
		UFMODBlueprintStatics::PlayEvent2D(this, m_step1Sound, true);
		//Play Anim
		break;
	case EBRICK_STATE::PREPARATION:
		Blink((errorWindow - activationWindow) / 1000.f);
		UFMODBlueprintStatics::PlayEvent2D(this, m_step2Sound, true);
		break;
	case EBRICK_STATE::HIT:
		Blink((activationWindow*2.f) / 1000.f);
		UFMODBlueprintStatics::PlayEvent2D(this, m_step3Sound, true);
		break;
	case EBRICK_STATE::PASSED:

		if (m_interactionState == EINTERACTION_RESULT::Valid)
		{
			FDamageEvent dmgEvent;
			m_enemy->TakeDamage(m_dynamicScoreValue, dmgEvent, nullptr, nullptr);
		}

		m_enemy->SetVisible(true, m_dissolveDuration);
		Hide();
		break;
	default:
		break;
	}
}

void ACoopBrick::Blink(float blinkduration)
{
	m_dissolveComponent->Ressolve(m_dissolveDuration);
	m_dissolveComponent->Dissolve(m_dissolveDuration, blinkduration - m_dissolveDuration);
}

void ACoopBrick::OnDeviateBoss()
{
	UFMODBlueprintStatics::PlayEvent2D(this, m_shockwaveSound, true);

	FVector socketLoc = m_player->GetSkeletalMeshComponent()->GetSocketLocation("HipSocket");

	m_shieldParticle->SetWorldLocation(socketLoc);
	m_shieldParticle->AttachToComponent(m_player->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	m_shieldParticle->SetTemplate(m_shieldParticleArray[(int)m_successType]);
	m_shieldParticle->ActivateSystem();
}

void ACoopBrick::SetSuccessType(ESUCCESS_TYPE success)
{
	Super::SetSuccessType(success);
	OnDeviateBoss();
}

void ACoopBrick::Show()
{
	Super::Show();
	m_mesh->SetVisibility(true);
}

void ACoopBrick::Hide()
{
	Super::Hide();
	m_mesh->SetVisibility(false);
}