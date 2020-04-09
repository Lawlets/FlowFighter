
#include "RailHandler.h"
#include "RhythmOfTheDemon.h"
#include "Actors/Rail.h"
#include "Sound/RhythmHandler.h"
#include "Engine/World.h"
#include "Actors/SplineLevel.h"
#include "DataStruct/LevelDataStruct.h"
#include "Actors/PlayerEntity.h"

#include "RhythmOfTheDemonGameStateBase.h"
#include "RhythmOfTheDemonGameModeBase.h"

ARailHandler* ARailHandler::m_instance;

ARailHandler::ARailHandler()
{
	PrimaryActorTick.bCanEverTick = true;
	m_playerRailArray = TArray<ARail*>();
	m_enemyRailArray = TArray<ARail*>();
	m_instance = this;
}

void ARailHandler::BeginPlay()
{
	Super::BeginPlay();
}

void ARailHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void ARailHandler::SetupSplineCurve(FSplineCurves spline_curve)
//{
//	UE_LOG(LogTemp, Warning, TEXT("SetupSpineCurve"));
//	m_currentRail->GetSpline()->SplineCurves = spline_curve;
//}

ARail* ARailHandler::GetPlayerRail() const
{
	return GetPlayerRail(m_railIndex);
}

ARail* ARailHandler::GetPlayerRail(int32 levelIndex) const
{
	if (levelIndex > m_playerRailArray.Max() || levelIndex < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("index is out of railArray size"));
		return nullptr;
	}

	return m_playerRailArray[levelIndex];
}

ARail* ARailHandler::GetEnemyRail() const
{
	return GetEnemyRail(m_railIndex);
}

ARail* ARailHandler::GetEnemyRail(int32 levelIndex) const
{
	if (levelIndex > m_enemyRailArray.Max() || levelIndex < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("index is out of railArray size"));
		return nullptr;
	}

	return m_enemyRailArray[levelIndex];
}

void ARailHandler::SetCurrentRailIndex(int32 railIndex)
{
	m_railIndex = railIndex;
}

void ARailHandler::ClearCurrentRailIndex()
{
	m_railIndex = -1;
}