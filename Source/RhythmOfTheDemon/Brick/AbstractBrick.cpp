// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractBrick.h"
#include "Brick/BricksHandler.h"
#include "Actors/GameplaySpaceAnchor.h"

//#include "Actors/Components/DissolveComponent.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

// Sets default values
AAbstractBrick::AAbstractBrick(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	m_root = CreateDefaultSubobject<USceneComponent>(TEXT("test"));
	RootComponent = m_root;

	this->RemoveInstanceComponent(m_dissolveComponent);
	m_dissolveComponent = CreateDefaultSubobject<UDissolveComponent>(FName("DissolveComponent"));
	this->AddInstanceComponent(m_dissolveComponent);
	//m_dissolveComponent->RegisterComponent();
}

// Called when the game starts or when spawned
void AAbstractBrick::BeginPlay()
{
	Super::BeginPlay();

	m_brickHdl = ABricksHandler::GetInstance();
	m_gameplaySpaceAnchor = AGameplaySpaceAnchor::GetInstance();

	Hide();
}

void AAbstractBrick::Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target)
{
	m_player = target;
	m_enemy = caster;

	m_interactionState = EINTERACTION_RESULT::None;
	m_isPerfectTimingPassed = false;

	m_creationTimelinePosition = m_timelinePosition - travelTime;
	m_travelTime = travelTime;

	SwitchState(EBRICK_STATE::IDLE);
}


// Called every frame
void AAbstractBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//m_dissolveComponent->Update(DeltaTime);
	//UE_LOG(ApplicationInitLog, Log, TEXT(" Brick Tick !"));
}

void AAbstractBrick::Update(int timelinePosition)
{
	UpdateState(timelinePosition);
}

EINTERACTION_RESULT AAbstractBrick::Interact(float timelinePos, bool isFromArmPlayer)
{
	switch (state)
	{
	case EBRICK_STATE::PREPARATION:
		m_brickInteraction.Broadcast(this);
		return EINTERACTION_RESULT::Miss;

	case EBRICK_STATE::HIT:
		//m_brickInteraction.Broadcast(this);
		return EINTERACTION_RESULT::Valid;

	default:
		return EINTERACTION_RESULT::None;
	}
}

void AAbstractBrick::Show()
{
	//SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	m_inUse = true;

	m_dissolveComponent->SetComponentTickEnabled(true);
}

void AAbstractBrick::Hide()
{
	//SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	m_inUse = false;

	m_dissolveComponent->SetComponentTickEnabled(false);
	m_dissolveComponent->OnDissolveComplete().RemoveAll(this);
}

void AAbstractBrick::UpdateState(int currTimelinePos)
{
	if (state == EBRICK_STATE::PASSED)
		return;

	int timelineDiff = m_timelinePosition - currTimelinePos;


	switch (state)
	{
	case EBRICK_STATE::IDLE:
		if (timelineDiff < errorWindow)
			SwitchState(EBRICK_STATE::PREPARATION);
		break;

	case EBRICK_STATE::PREPARATION:
		if (timelineDiff < activationWindow)
			SwitchState(EBRICK_STATE::HIT);
		break;

	case EBRICK_STATE::HIT:
		if (timelineDiff < 0 && !m_isPerfectTimingPassed)
			PerfectTiming();

		if (timelineDiff < -activationWindow)
			SwitchState(EBRICK_STATE::PASSED);
		break;

	default:
		break;
	}
}

void AAbstractBrick::SwitchState(EBRICK_STATE newState)
{
	state = newState;
	//UE_LOG(ApplicationInitLog, Log, TEXT("* brick Switch to state n°%i"), (int)state);

	switch (newState)
	{
	case EBRICK_STATE::IDLE:
		m_idleStateEvent.Broadcast(this);
		if (m_dynamicMaterial != nullptr)
		{
			m_dynamicMaterial->SetVectorParameterValue(FName("Color"), m_idleColor);
			m_dynamicMaterial->SetScalarParameterValue(FName("Emissive_force"), m_idleEmissiveForce);
		}
		break;
	case EBRICK_STATE::PREPARATION:
		m_preparationStateEvent.Broadcast(this);
		if (m_dynamicMaterial != nullptr)
		{
			m_dynamicMaterial->SetVectorParameterValue(FName("Color"), m_prepColor);
			m_dynamicMaterial->SetScalarParameterValue(FName("Emissive_force"), m_prepEmissiveForce);
		}
		break;
	case EBRICK_STATE::HIT:
		m_hitStateEvent.Broadcast(this);
		if (m_dynamicMaterial != nullptr)
		{
			m_dynamicMaterial->SetVectorParameterValue(FName("Color"), m_hitColor);
			m_dynamicMaterial->SetScalarParameterValue(FName("Emissive_force"), m_hitEmissiveForce);
		}
		break;
	case EBRICK_STATE::PASSED:
		m_passedStateEvent.Broadcast(this);
		break;
	default:
		break;
	}
}

void AAbstractBrick::PerfectTiming()
{
	m_isPerfectTimingPassed = true;
	m_perfectTimingEvent.Broadcast(this);
}

void AAbstractBrick::BroadcastBrickInteraction()
{
	m_brickInteraction.Broadcast(this);
}