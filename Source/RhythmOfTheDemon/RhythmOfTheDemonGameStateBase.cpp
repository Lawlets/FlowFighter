// Fill out your copyright notice in the Description page of Project Settings.

#include "RhythmOfTheDemonGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "Sound/RhythmHandler.h"
#include "Actors/CameraHandler.h"
#include "Actors/RailHandler.h"
#include "Actors/ScoringHandler.h"
#include "Actors/TutorialHandler.h"

#include "Actors/PlayerEntity.h"
#include "AI/MiniBossPawn.h"

#include "RhythmOfTheDemonGameModeBase.h"

ARhythmOfTheDemonGameStateBase::ARhythmOfTheDemonGameStateBase()
{
}

void ARhythmOfTheDemonGameStateBase::HandleBeginPlay()
{
	Super::HandleBeginPlay();

	m_RotDGameMode = Cast<ARhythmOfTheDemonGameModeBase>(GetWorld()->GetAuthGameMode());

	m_tutoHandler = ATutorialHandler::GetInstance();
	m_rhythmHandler = ARhythmHandler::GetInstance();
	m_cameraHandler = m_RotDGameMode->GetCameraHandler();
	m_scoringHandler = AScoringHandler::GetInstance();
	m_railHandler = ARailHandler::GetInstance();
	m_brickHandler = ABricksHandler::GetInstance();
	//InitGame();
}

void ARhythmOfTheDemonGameStateBase::InitGame()
{
	InitGame_Event();
	LoadMenu(false, false);
}

void ARhythmOfTheDemonGameStateBase::OnLoadLevel()
{
	OnLoadLevel_Event();
}

void ARhythmOfTheDemonGameStateBase::PauseGame()
{
	if(BlockPause())
		return;
	//m_cameraHandler->BlendTo(ECAMERA_VIEW::PAUSE);
	bool isFading = m_cameraHandler->GetFadingState();
	if (m_isPause)
		return;

	m_isPause = true;
	PauseMusic();
	Pause_Event();
}

bool ARhythmOfTheDemonGameStateBase::BlockPause()
{
	//APlayerEntity* player = m_RotDGameMode->GetPlayer();
	//
	//if (player->GetLife() <= 0.f)
	//	return true;

	//if (m_isLevelEnded )
	//	return true;

	return m_isLevelEnded;
}

void ARhythmOfTheDemonGameStateBase::ResumeGame()
{
	//m_cameraHandler->BlendTo(ECAMERA_VIEW::TPS);
	Resume_Event();
	//ResumeMusic();
}

void ARhythmOfTheDemonGameStateBase::DisplayMenuUI()
{
	DisplayMenuUI_Event();
}

bool ARhythmOfTheDemonGameStateBase::CheckEndGameCondition()
{
	AMiniBossPawn* enemy = m_RotDGameMode->GetEnemy();
	if (enemy == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Enemy is nullptr in CheckEndGameCondition: Error"));

	if (enemy->GetLife() > 0.f)
		return false;

	return true;
}

void ARhythmOfTheDemonGameStateBase::OnLevelEnd()
{
	if (CheckEndGameCondition())
		Win();
	else
		Lose();
}

void ARhythmOfTheDemonGameStateBase::Win()
{
	LevelEndRoutine();

	Win_Event();
}

void ARhythmOfTheDemonGameStateBase::Lose()
{
	LevelEndRoutine();

	Lose_Event();
}

void ARhythmOfTheDemonGameStateBase::LevelEndRoutine()
{
	PauseMusic();
	m_brickHandler->ClearBricks();
	m_isLevelEnded = true;

	// rotate camera
	StopTimeEffect();
}

void ARhythmOfTheDemonGameStateBase::StopTimeEffect()
{
	// post effect on camera (black&white)
	// time scale / x ?
}

void ARhythmOfTheDemonGameStateBase::GoToMenu()
{
	GoToMenu_Event();
}

void ARhythmOfTheDemonGameStateBase::PauseMusic()
{
	m_rhythmHandler->Pause(true);
}

void ARhythmOfTheDemonGameStateBase::ResumeMusic()
{
	m_rhythmHandler->Pause(false);
}

void ARhythmOfTheDemonGameStateBase::InitLevel()
{
	
	//Clear Old Level
	m_RotDGameMode->ClearCurrentLevel();
	m_rhythmHandler->StopMusic();
	
	int oldLevelIndex = m_currentLevelIndex;

	// Load new Level Data
	UDataTable* dataTable = m_RotDGameMode->LevelDatas;
	FLevelDataStruct* levelToLoad = dataTable->FindRow<FLevelDataStruct>(m_levelToLoad, nullptr);
	m_currentLevelIndex = levelToLoad->levelIndex;

	FLatentActionInfo LoadInfo;
	FString levelToLoad_string = FString("LightLevel_lvl") + FString::FromInt(m_currentLevelIndex);
	FName levelToLoad_name = *levelToLoad_string;
	UGameplayStatics::LoadStreamLevel(GetWorld(), levelToLoad_name, true, true, LoadInfo);

	// Setup New Level
	m_RotDGameMode->InstanciateEnemy(levelToLoad->enemyActor);

	APlayerEntity* player = m_RotDGameMode->GetPlayer();
	AMiniBossPawn* enemy = m_RotDGameMode->GetEnemy();

	// Setup Handler
	m_scoringHandler->Init(m_RotDGameMode->GetGameDifficulties());
	m_railHandler->SetCurrentRailIndex(levelToLoad->levelIndex);
	m_brickHandler->Init(player, enemy, levelToLoad);

	if (levelToLoad->isTutorial)
		m_tutoHandler->Init();

	// Setup Entity
	player->ClearPlayerInput();
	player->Init(m_railHandler->GetPlayerRail(), levelToLoad->speedCurve);
	enemy->Init(m_railHandler->GetEnemyRail());

	m_cameraHandler->BlendTo(ECAMERA_VIEW::END_LEVEL, true);
}

void ARhythmOfTheDemonGameStateBase::StartLevel()
{
	m_RotDGameMode->GetPlayer()->SetPlayerInput();

	UDataTable* dataTable = m_RotDGameMode->LevelDatas;
	FLevelDataStruct* levelToLoad = dataTable->FindRow<FLevelDataStruct>(m_levelToLoad, nullptr);
	m_rhythmHandler->InitAudio(levelToLoad);
	m_rhythmHandler->Pause(false);
	OnLoadLevel();
	m_rhythmHandler->PlayMusic();
	m_isLevelEnded = false;
	m_isPause = false;

	//Difficulty
	int fmodDifficulty = (int)(m_RotDGameMode->GetGameDifficulties()) + 1;
	m_rhythmHandler->SetParameter("Difficulty", fmodDifficulty);
}

void ARhythmOfTheDemonGameStateBase::SetupMenuLevel()
{
	ARhythmHandler* rhythmHandler = ARhythmHandler::GetInstance();
	rhythmHandler->Pause(true);
	rhythmHandler->StopMusic();

	ARailHandler* railHandler = ARailHandler::GetInstance();
	railHandler->ClearCurrentRailIndex();

	m_RotDGameMode->ClearCurrentLevel();
	APlayerEntity* player = m_RotDGameMode->GetPlayer();
	player->SetActorLocation(FVector(82.f, 101.f, 22.f));
	FRotator rotator = FRotator(0.f, 90.f, 0.f);
	player->SetActorRotation(rotator);
	player->Reset();
	m_cameraHandler->BlendTo(ECAMERA_VIEW::MENU_IDLE, true);
	m_levelToLoad = TEXT("");

	GoToMenu();
	DisplayMenuUI();
}

void ARhythmOfTheDemonGameStateBase::LoadLevel(FName levelName, bool useFade)
{
	m_cameraHandler->FadeInEnd.Clear();
	m_cameraHandler->FadeOutEnd.Clear();

	m_cameraHandler->FadeInEnd.RemoveAll(this);
	m_cameraHandler->FadeOutEnd.RemoveAll(this);

	m_levelToLoad = levelName;
	m_scoringHandler->ResetScore();

	m_RotDGameMode->GetPlayer()->ClearPlayerInput();
	
	UDataTable* dataTable = m_RotDGameMode->LevelDatas;
	FLevelDataStruct* levelToLoad = dataTable->FindRow<FLevelDataStruct>(m_levelToLoad, nullptr);
	int oldLevelIndex = m_currentLevelIndex;
	int newLevelIndex = levelToLoad->levelIndex;

	//if (oldLevelIndex == -1)
	//	oldLevelIndex = 0;

	if (oldLevelIndex != -1)
	{
		FLatentActionInfo UnloadInfo;
		FString levelToUnload_string = FString("LightLevel_lvl") + FString::FromInt(oldLevelIndex);
		FName levelToUnload_name = *levelToUnload_string;
		UGameplayStatics::UnloadStreamLevel(GetWorld(), levelToUnload_name, UnloadInfo, true);
	}

	m_tutoHandler->EndTutorial();

	if (useFade)
	{
		m_cameraHandler->FadeOutEnd.AddLambda([this] {
			InitLevel();

			m_cameraHandler->FadeInEnd.AddLambda([this] {
				StartLevel();
			});

			m_cameraHandler->FadeInCurrentCamera();
		});

		m_cameraHandler->FadeOutCurrentCamera();
	}
	else
	{
		InitLevel();
		StartLevel();
	}
}


void ARhythmOfTheDemonGameStateBase::LoadMenu(bool useFade, bool clearInput)
{
	m_brickHandler->ClearBricks();

	m_cameraHandler->FadeInEnd.Clear();
	m_cameraHandler->FadeOutEnd.Clear();

	m_cameraHandler->FadeInEnd.RemoveAll(this);
	m_cameraHandler->FadeOutEnd.RemoveAll(this);

	int oldLevelIndex = m_currentLevelIndex;

	m_currentLevelIndex = -1;
	m_scoringHandler->ResetScore();


	if (oldLevelIndex == -1)
		oldLevelIndex = 0;

	FLatentActionInfo UnloadInfo;
	FString levelToUnload_string = FString("LightLevel_lvl") + FString::FromInt(oldLevelIndex);
	FName levelToUnload_name = *levelToUnload_string;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), levelToUnload_name, UnloadInfo, true);

	if (clearInput)
		m_RotDGameMode->GetPlayer()->ClearPlayerInput();

	if (useFade)
	{
		m_cameraHandler->FadeOutEnd.AddLambda([this] {
			SetupMenuLevel();
			m_cameraHandler->FadeInCurrentCamera();
		});
		m_cameraHandler->FadeOutCurrentCamera();
	}
	else
		SetupMenuLevel();

}