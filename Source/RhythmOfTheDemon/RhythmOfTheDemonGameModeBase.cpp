// Fill out your copyright notice in the Description page of Project Settings.

#include "RhythmOfTheDemonGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AI/EnemyPawn.h"
#include "Engine/World.h"
#include "Actors/PlayerEntity.h"
#include "AI/MiniBossPawn.h"
#include "Actors/RailHandler.h"
#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "RhythmOfTheDemonGameStateBase.h"
#include "Actors/CameraHandler.h"
#include "DataStruct/LevelDataStruct.h"
#include "HAL/Event.h"
#include "Internationalization/Internationalization.h"

#include "EngineUtils.h"

ARhythmOfTheDemonGameModeBase::ARhythmOfTheDemonGameModeBase()
{
}


void ARhythmOfTheDemonGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Role = ROLE_Authority;
	m_gameDifficulties = EDIFFICULTIES::Easy;

	APlayerController* second_controller = (UGameplayStatics::GetPlayerController(GetWorld(), 1) == nullptr) ? UGameplayStatics::CreatePlayer(GetWorld(), 1) : UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (second_controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is null"));
		return;
	}

	for (TActorIterator<ACameraHandler> CameraHandlerIterator(GetWorld()); CameraHandlerIterator; ++CameraHandlerIterator)
	{
		m_cameraHandler = *CameraHandlerIterator;
		break;
	}

	if (m_cameraHandler == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("CameraHandler is null"));

	if (PlayerActor != nullptr)
		InstanciatePlayer();

	//if (AIActor != nullptr)
	//	InstanciateEnemy(AIActor);

	//m_cameraHandler->AddCameraActorToMap(ECAMERA_VIEW::TPS, m_player->GetMainCameraChildActor());
	m_cameraHandler->AddCameraActorToMap(ECAMERA_VIEW::QUARTER_RIGHT, m_player->GetQuarterRightCameraChildActor());
	m_cameraHandler->AddCameraActorToMap(ECAMERA_VIEW::QUARTER_LEFT, m_player->GetQuarterLeftCameraChildActor());
	m_cameraHandler->AddCameraActorToMap(ECAMERA_VIEW::PAUSE, m_player->GetPauseCameraChildActor());
	m_cameraHandler->AddCameraActorToMap(ECAMERA_VIEW::END_LEVEL, m_player->GetMenuCameraChildActor());
	
	m_cameraHandler->BlendTo(ECAMERA_VIEW::MENU_IDLE, true);
}

void ARhythmOfTheDemonGameModeBase::InstanciatePlayer()
{
	FActorSpawnParameters parameters;

	for (TActorIterator<APlayerEntity> PlayersIterator(GetWorld()); PlayersIterator; ++PlayersIterator)
	{
		m_player = *PlayersIterator;
		break;
	}

	if (m_player == nullptr)
	{
		parameters.Name = TEXT("PlayerEntity");
		parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* player = GetWorld()->SpawnActor<AActor>(PlayerActor, FVector(82.f, 101.f, 22.f), FRotator(0.f, 0.f, -90.f), parameters);
		m_player = Cast<APlayerEntity>(player);
	}
}

void ARhythmOfTheDemonGameModeBase::InstanciateEnemy(TSubclassOf<AActor> enemyActor)
{
	if (enemyActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Instanciate enemey from a nullptr actor"));
		return;
	}

	FActorSpawnParameters parameters;
	parameters.Name = TEXT("AIMiniBoss");
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMiniBossPawn* miniBossPawn = GetWorld()->SpawnActor<AMiniBossPawn>(enemyActor, parameters);
	m_enemy = miniBossPawn;

	if (m_enemy != nullptr && m_player != nullptr)
		m_enemy->m_player = m_player;
}

void ARhythmOfTheDemonGameModeBase::ClearCurrentLevel()
{
	if (m_enemy != nullptr)
	{
		m_enemy->Destroy();
		m_enemy = nullptr;
	}

	if (m_player != nullptr)
	{
		m_player->Reset();
		m_cameraHandler->ReplaceCameraActorInMap(ECAMERA_VIEW::QUARTER_RIGHT, m_player->GetQuarterRightCameraChildActor());
	}
}

APlayerEntity* ARhythmOfTheDemonGameModeBase::GetPlayer()
{
	return m_player;
}

AMiniBossPawn* ARhythmOfTheDemonGameModeBase::GetEnemy()
{
	return m_enemy;
}

void ARhythmOfTheDemonGameModeBase::ChangeCulture(const FString cultureName)
{
	FInternationalization::Get().SetCurrentCulture(cultureName);
}

int ARhythmOfTheDemonGameModeBase::GetOctaveNbFromCurrentDifficulty() const
{
	switch (m_gameDifficulties)
	{
	case EDIFFICULTIES::Easy:
		return 3;
	case EDIFFICULTIES::Normal:
		return 5;
	case EDIFFICULTIES::Hard:
		return 7;
	default:
		return 3;
	}
}