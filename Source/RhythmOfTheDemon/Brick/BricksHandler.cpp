// Fill out your copyright notice in the Description page of Project Settings.

#include "BricksHandler.h"
#include "AbstractProjectile.h"
#include "CoopBrick.h"

#include "Actors/PlayerEntity.h"
#include "AI/EnemyPawn.h"
#include "AI/MiniBossPawn.h"

#include "Sound/RhythmHandler.h"
#include "Actors/ScoringHandler.h"
#include "Actors/FeedbackHandler.h"
//#include "Actors/RailHandler.h"
//#include "Actors/Rail.h"

#include "DataStruct/BrickPatternData.h"
#include "Enums/InteractionResult.h"
#include "Enums/SuccessType.h"

#include "FMODBlueprintStatics.h"
#include "Components/SplineComponent.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"
#include "RhythmOfTheDemonGameModeBase.h"

ABricksHandler* ABricksHandler::m_instance;

// Sets default values
ABricksHandler::ABricksHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_instance = this;
}

// Called when the game starts or when spawned
void ABricksHandler::BeginPlay()
{
	Super::BeginPlay();

	m_rhythmHdl = ARhythmHandler::GetInstance();
	m_scoringHandler = AScoringHandler::GetInstance();
	m_feedbackHdl = AFeedbackHandler::GetInstance();

	//BrickArray
	int brickTypeCount = (int)EACTION_MAPPING::Count;
	m_brickArrayByType.Init(TArray<AAbstractBrick*>(), brickTypeCount);


	//Put all that into pool
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//NewPool
	int currPoolSizePerBrick = poolSizePerBricks;
	for (int i = 0; i < typeArray.Num(); i++)
	{
		TSubclassOf<AAbstractBrick> currType = typeArray[i];
		if (i == typeArray.Num() - 1) // HotFix for coopbrick
			currPoolSizePerBrick = 2;
		for (int j = 0; j < currPoolSizePerBrick; j++)
		{
			AAbstractBrick* brick = GetWorld()->SpawnActor<AAbstractBrick>(currType, param);
			brick->Hide();
			obstaclePool.Add(brick);
		}
	}
}

// PUBLIC

void ABricksHandler::Init(APlayerEntity* player, AMiniBossPawn* enemy, FLevelDataStruct* levelData)
{
	m_player = player;
	m_enemy = enemy;
	InitLevelObstacles(levelData);
}

void ABricksHandler::ClearBricks()
{
	//Move to Pool
	for (int i = 0; i < obstaclePool.Num(); i++)
	{
		obstaclePool[i]->Hide();
	}

	for (int i = 0; i < m_brickArrayByType.Num(); i++)
	{
		m_brickArrayByType[i].Reset();
	}
}

// Called every frame
void ABricksHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int currTimelinePosition = m_rhythmHdl->GetTimelinePosition();

	if (DisableBricks)
		return;

	//Manage Instantiation
	CheckForBrickInstantiation(currTimelinePosition);

	//Manage Update 
	for (int i = 0; i < m_brickArrayByType.Num(); i++)
		UpdateObstaclesFromArray(m_brickArrayByType[i], currTimelinePosition);

	// Compute Brick
	int interactableBrickType = (int)EACTION_MAPPING::None;
	for (int i = 0; i < interactableBrickType; i++)
		ComputeBrickRepercussionFromArray(m_brickArrayByType[i]);

	RemoveUnusedBricks(m_brickArrayByType[MANAGED_BRICK_ID]);
}

// PRIVATE

void ABricksHandler::InitLevelObstacles(FLevelDataStruct* levelData)
{
	// Clean and just get Enemy
	ARhythmOfTheDemonGameModeBase* gameMode = (ARhythmOfTheDemonGameModeBase*)GetWorld()->GetAuthGameMode();
	int targetOctave = gameMode->GetOctaveNbFromCurrentDifficulty();

	//Hide every old obstacle
	ClearBricks();

	// Init new Obstacle
	TArray<FBrickDataStruct> futureObstacle = midiTranslator.TranslateMidi(levelData->midiFileName, MidiBrickMap, targetOctave);

	m_futureObstacle.Reset();
	m_futureObstacle.Append(futureObstacle);
}


//TODO May need Improvement to manage Multiple Instantiation at Once
void ABricksHandler::CheckForBrickInstantiation(int timelinePos)
{
	if (m_futureObstacle.Num() > 0)
	{
		int futureTimelinePosition = timelinePos + brickTravelDuration;

		if (m_futureObstacle[0].TimelinePosition <= futureTimelinePosition)
		{
			InstantiateObstacle(m_futureObstacle[0]);
			m_futureObstacle.RemoveAt(0);
		}
	}
}

void ABricksHandler::InstantiateObstacle(FBrickDataStruct brickData)
{
	AAbstractBrick* newBrick = FindUnusedBrick(brickData.brickClass);
	if (newBrick == nullptr)
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" Unable to find any brick of class %s"), brickData.brickClass);
		return;
	}

	newBrick->Show();
	newBrick->SetTimelinePosition(brickData.TimelinePosition);
	newBrick->Init(brickTravelDuration, m_enemy, m_player);

	m_brickArrayByType[(int)newBrick->GetType()].Add(newBrick);
	//UE_LOG(ApplicationInitLog, Log, TEXT(" add brick to array of Id = %i"), (int)newBrick->GetType());

	//Event
	m_brickInstantiationEvent.Broadcast(newBrick);

	//Anim
	m_enemy->CastProjectiles(newBrick);
}

void ABricksHandler::UpdateObstaclesFromArray(TArray<AAbstractBrick*> obstacleArray, float currTimelinePosition)
{
	int brickCount = obstacleArray.Num();
	if (brickCount > 0)
	{
		for (int32 i = 0; i < brickCount; i++)
		{
			if (obstacleArray[i]->IsInUse())
				obstacleArray[i]->Update(currTimelinePosition);
		}
	}
}

void ABricksHandler::Interact(EACTION_MAPPING action, bool isFromArmPlayer)
{
	TArray<AAbstractBrick*>& bricks = m_brickArrayByType[int(action)];

	//UE_LOG(ApplicationInitLog, Log, TEXT(" interact with brick in array of Id = %i"), (int)action);

	if (bricks.Num() <= 0)
		return;

	//UE_LOG(ApplicationInitLog, Log, TEXT("* check interactable ..."));


	AAbstractBrick* firstBrick = bricks[0];
	if (!firstBrick->IsInteractable()) // Hotfix => ugly one
	{
		for (int i = 0; i < bricks.Num(); i++)// Find next interactable
		{
			firstBrick = bricks[i];
			if (firstBrick->IsInteractable())
				break;
		}
		if (!firstBrick->IsInteractable()) // if not found any
			return;
	}

	int currTimelinePos = m_rhythmHdl->GetTimelinePosition();
	EINTERACTION_RESULT result = firstBrick->Interact(m_rhythmHdl->GetTimelinePosition(), isFromArmPlayer);

	//UE_LOG(ApplicationInitLog, Log, TEXT("* curr timeline = %i"), currTimelinePos);
	//UE_LOG(ApplicationInitLog, Log, TEXT("* brick timepos = %i"), firstBrick->GetTimelinePosition());

	// Manage Interaction Result
	if (result == EINTERACTION_RESULT::Valid)
	{
		PositiveFeedback(firstBrick);
	}
	else if (result == EINTERACTION_RESULT::Miss)
	{
		NegativeFeedback(firstBrick);
	}
}

//Should be in a Pool
AAbstractBrick* ABricksHandler::FindUnusedBrick(TSubclassOf<AAbstractBrick> subclass)
{
	int index = 0;
	bool found = typeArray.Find(subclass, index);

	if (!found)
	{
		UE_LOG(ApplicationInitLog, Log, TEXT("* Cannot find subclass %s in the typeArray of brick handler ... "), *(subclass.Get()->GetFName()).ToString());
		return nullptr;
	}

	int startIndex = index * poolSizePerBricks;

	for (int i = startIndex; i < startIndex + poolSizePerBricks; i++)
	{
		if (obstaclePool[i]->IsInUse() == false)
		{
			return obstaclePool[i];
		}
	}
	UE_LOG(ApplicationInitLog, Log, TEXT("* Cannot find any free brick of type %s in the pool ... "), *(subclass.Get()->GetFName()).ToString());
	return nullptr;
}

void ABricksHandler::ComputeBrickRepercussionFromArray(TArray<AAbstractBrick*>& bricks)
{
	if (bricks.Num() <= 0)
		return;

	AAbstractBrick* currBrick = bricks[0];
	if (currBrick->GetState() == EBRICK_STATE::PASSED)
	{
		ComputeBrickRepercussion(currBrick);

		//Remove Managed Brick
		bricks.Remove(currBrick);
		m_brickArrayByType[MANAGED_BRICK_ID].Add(currBrick);
	}
}

void ABricksHandler::RemoveUnusedBricks(TArray<AAbstractBrick*>& bricks)
{
	if (bricks.Num() <= 0)
		return;

	for (int i = 0; i < bricks.Num(); i++)
	{
		if (bricks[i]->IsInUse() == false)
		{
			bricks.RemoveAt(i);
			break;
		}
	}
}

void ABricksHandler::PositiveFeedback(AAbstractBrick* brick)
{
	ESUCCESS_TYPE successType = m_scoringHandler->ComputeSuccessType(m_rhythmHdl->GetTimelinePosition(), brick);
	float successScore = m_scoringHandler->ComputeScore(brick->GetScoreValue(), successType);
	brick->SetDynamicScoreValue(successScore);
	brick->SetSuccessType(successType);

	//UE_LOG(ApplicationInitLog, Log, TEXT(" Success with %f Score"), successScore);
	m_scoringHandler->GainScore(brick);
	//UE_LOG(ApplicationInitLog, Log, TEXT("* Brick State when positive = %i"), (int)brick->GetInteractionState());
	// Juicy ? :3
	// Feedback
	m_feedbackHdl->ShowTextFeedback(successType, brick->GetTargetType());
}

void ABricksHandler::NegativeFeedback(AAbstractBrick* brick)
{
	m_feedbackHdl->ShakeController(brick->GetTargetType(), m_onErrorControllerShake);

	//UE_LOG(ApplicationInitLog, Log, TEXT(" Fail"));
	m_feedbackHdl->ShowTextFeedback(ESUCCESS_TYPE::Miss, brick->GetTargetType()); // maybe try a specific text ? "Too Early" ?
}

void ABricksHandler::NoInteractionFeedback(AAbstractBrick* brick)
{
	m_feedbackHdl->ShakeController(brick->GetTargetType(), m_onErrorControllerShake);

	//UE_LOG(ApplicationInitLog, Log, TEXT(" Miss"));
	m_feedbackHdl->ShowTextFeedback(ESUCCESS_TYPE::Miss, brick->GetTargetType());
}

void ABricksHandler::ComputeBrickRepercussion(AAbstractBrick* brick)
{
	//UE_LOG(ApplicationInitLog, Log, TEXT("* Brick State when passed = %i"), (int)brick->GetInteractionState());
	if (brick->GetInteractionState() != EINTERACTION_RESULT::Valid) // Error
	{
		ComputeBadInteraction(brick);
	}
	else // Valid
	{
		ComputeGoodInteraction(brick);
	}
}

void ABricksHandler::ComputeGoodInteraction(AAbstractBrick* brick)
{
	//UE_LOG(ApplicationInitLog, Log, TEXT("* scoreGained = %f"), scoreGained);
	m_player->Regenerate(brick->GetDynamicScoreValue());
}

void ABricksHandler::ComputeBadInteraction(AAbstractBrick* brick)
{
	if (brick->GetInteractionState() == EINTERACTION_RESULT::None) // Trop tard
		NoInteractionFeedback(brick);

	float damage = m_scoringHandler->ComputeDamageScore(brick);

	FDamageEvent dmgEvent;
	m_player->TakeDamage(damage, dmgEvent, nullptr, nullptr);
}