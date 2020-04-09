// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialHandler.h"
#include "Engine.h"

#include "RhythmOfTheDemonGameModeBase.h"
#include "Brick/BricksHandler.h"
#include "Actors/VisualEffectHandler.h"
#include "Sound/RhythmHandler.h"
#include "Actors/FeedbackHandler.h"

#include "Actors/PlayerEntity.h"
#include "Characters/ArmPlayer.h"
#include "Characters/LegPlayer.h"

ATutorialHandler* ATutorialHandler::m_instance;

// Sets default values
ATutorialHandler::ATutorialHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_instance = this;
}

// Called when the game starts or when spawned
void ATutorialHandler::BeginPlay()
{
	Super::BeginPlay();

	m_bricksHdl = ABricksHandler::GetInstance();
	m_rhythmHdl = ARhythmHandler::GetInstance();
	m_feedbackHdl = AFeedbackHandler::GetInstance();

	m_tutoBrickArray = TArray<AAbstractBrick*>();

	SetActorTickEnabled(false);
}

// Called every frame
void ATutorialHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int currTimePos = m_rhythmHdl->GetTimelinePosition();
	//UE_LOG(ApplicationInitLog, Log, TEXT("* tuto tick with timepos = %i"), currTimePos);

	if (currTimePos >= m_tutoStepTiming[m_currTutoStepIdx])
	{
		m_feedbackHdl->ShakeController((EPLAYER_TYPE)m_currTutoStepIdx);
		ShowTutoStepFeedback(m_currTutoStepIdx, m_stepTextDuration);
		m_currTutoStepIdx++;

		if (m_currTutoStepIdx >= m_tutoStepTiming.Num())
			SetActorTickEnabled(false);
	}

}

void ATutorialHandler::Init()
{
	ARhythmOfTheDemonGameModeBase* RotDGameMode = Cast<ARhythmOfTheDemonGameModeBase>(GetWorld()->GetAuthGameMode());
	m_player = RotDGameMode->GetPlayer();

	m_tutoBrickArray.Empty();
	m_tutoBrickArray.SetNum((int)EACTION_MAPPING::Count);

	m_bricksHdl->OnBrickInstantiation().AddUObject(this, &ATutorialHandler::ManageTutorialBrick);
	tutoPhaseCount = 0;
	m_currTutoStepIdx = 0;

	SetActorTickEnabled(true);
}

void ATutorialHandler::EndTutorial()
{
	if (!IsActorTickEnabled())
		return;

	for (int i = 0; i < m_tutoBrickArray.Num(); i++)
	{
		if (m_tutoBrickArray[i])
			m_tutoBrickArray[i]->CustomTimeDilation = 1.f;
	}

	m_rhythmHdl->SetParameter("Vitesse", 1.f);
	m_isTutorialFreezingTime = false;

	UnlockInput(EPLAYER_TYPE::Both);
	HideInputUI();
	SetActorTickEnabled(false);
	m_bricksHdl->OnBrickInstantiation().RemoveAll(this);
}

bool ATutorialHandler::IsTutorialDone()
{
	int count = 0;
	for (int i = 0; i < m_tutoBrickArray.Num(); i++)
	{
		if (m_tutoBrickArray[i] != nullptr)
			count++;
	}

	if (count >= m_BrickCountToCheck)
		return true;

	return false;
}


void ATutorialHandler::ManageTutorialBrick(AAbstractBrick* brick)
{
	int index = (int)brick->GetType();
	if (IsTutorialDone())
	{
		EndTutorial();
		return;
	}

	if (m_tutoBrickArray[index] == nullptr)
	{
		m_tutoBrickArray[index] = brick;

		//AddEvent
		brick->OnPreparationStateEvent().AddUObject(this, &ATutorialHandler::BeginTutoPhase);
		brick->OnHitStateEvent().AddUObject(this, &ATutorialHandler::ShowCurrentInput);
		brick->OnPerfectTimingEvent().AddUObject(this, &ATutorialHandler::FreezePhase);

		brick->OnBrickInteraction().AddUObject(this, &ATutorialHandler::EndTutoPhase);
	}
}

void ATutorialHandler::BeginTutoPhase(AAbstractBrick* brick)
{
	UE_LOG(ApplicationInitLog, Log, TEXT("* BeginTutoPhase"));
	tutoPhaseCount++;
	ShowInputUI(EACTION_MAPPING::None); // Show up a huge Wait ?? 
	LockInput(EPLAYER_TYPE::Both, EACTION_MAPPING::None); // Full lock

	if (brick->GetType() != EACTION_MAPPING::CastSoundWave)
		SlowActorTime(brick, brick->GetErrorWindow() / 1000.f);
}

void ATutorialHandler::ShowCurrentInput(AAbstractBrick* brick)
{
	UE_LOG(ApplicationInitLog, Log, TEXT("* ShowCurrentInput"));
	HideInputUI();
	ShowInputUI(brick->GetType());
	LockInput(brick->GetTargetType(), brick->GetType());
}

void ATutorialHandler::FreezePhase(AAbstractBrick* brick)
{
	UE_LOG(ApplicationInitLog, Log, TEXT("* FreezePhase"));
	m_isTutorialFreezingTime = true;
	m_rhythmHdl->Pause(true);

	if (brick->GetType() == EACTION_MAPPING::CastSoundWave)
		SlowActorTime(brick, 0.1f);
}

void ATutorialHandler::EndTutoPhase(AAbstractBrick* brick)
{
	UE_LOG(ApplicationInitLog, Log, TEXT("* EndTutoPhase"));
	HideInputUI();
	m_isTutorialFreezingTime = false;
	m_rhythmHdl->Pause(false);
	UnSlowActorTime(brick, brick->GetActivationWindow() / 1000.f);
	UnlockInput(EPLAYER_TYPE::Both);


	//RemoveEvent
	brick->OnPreparationStateEvent().RemoveAll(this);
	brick->OnHitStateEvent().RemoveAll(this);
	brick->OnPerfectTimingEvent().RemoveAll(this);
	brick->OnBrickInteraction().RemoveAll(this);;
}

void ATutorialHandler::LockInput(EPLAYER_TYPE target, EACTION_MAPPING action)
{
	if (target == EPLAYER_TYPE::Arm || target == EPLAYER_TYPE::Both)
	{
		m_player->GetArmPlayerPawn()->BindSpecificInput(action);
	}
	if (target == EPLAYER_TYPE::Leg || target == EPLAYER_TYPE::Both)
	{
		m_player->GetLegPlayerPawn()->BindSpecificInput(action);
	}
}

void ATutorialHandler::UnlockInput(EPLAYER_TYPE target)
{
	if (target == EPLAYER_TYPE::Arm || target == EPLAYER_TYPE::Both)
	{
		AAbstractPlayerBody* arm = m_player->GetArmPlayerPawn();
		arm->UnbindInput();
		arm->BindInput();
	}
	if (target == EPLAYER_TYPE::Leg || target == EPLAYER_TYPE::Both)
	{
		AAbstractPlayerBody* leg = m_player->GetLegPlayerPawn();
		leg->UnbindInput();
		leg->BindInput();
	}
}

void ATutorialHandler::ChangeActorTimeDilatationTo(AActor* actor, float value)
{
	actor->CustomTimeDilation = value;
	m_rhythmHdl->SetParameter("Vitesse", value);
}
