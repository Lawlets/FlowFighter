// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DataStruct/LevelDataStruct.h"
#include "RhythmOfTheDemonGameStateBase.generated.h"

class ARhythmOfTheDemonGameModeBase;
class UPlayerInput;

class ACameraHandler;
class ARhythmHandler;
class AScoringHandler;
class ATutorialHandler;
class ARailHandler;
class ABricksHandler;

/**
 *
 */
UCLASS()
class RHYTHMOFTHEDEMON_API ARhythmOfTheDemonGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	ARhythmOfTheDemonGameStateBase();
	virtual void HandleBeginPlay() override;

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void CheckEndGameCondition_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void Win_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void Lose_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void GoToMenu_Event();

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void Pause_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void Resume_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void DisplayMenuUI_Event();

	UFUNCTION(BlueprintCallable)
		void InitGame();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void InitGame_Event();

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void OnLoadLevel_Event();
	UFUNCTION(blueprintCallable)
		void OnLoadLevel();

	UFUNCTION(BlueprintCallable)
		void PauseGame();
	UFUNCTION(BlueprintCallable)
		void ResumeGame();

	UFUNCTION(BlueprintCallable)
		void DisplayMenuUI();

	bool CheckEndGameCondition();
	bool BlockPause();
	UFUNCTION(BlueprintCallable)
		void Win();
	UFUNCTION(BlueprintCallable)
		void Lose();
	UFUNCTION(BlueprintCallable)
		void GoToMenu();

	UFUNCTION(BlueprintCallable)
		void OnLevelEnd();

	UFUNCTION(BlueprintCallable)
		int GetCurrentLevelIndex() const { return m_currentLevelIndex; }

	UFUNCTION(BlueprintCallable)
		bool IsGamePause() const { return m_isPause; }

	//UFUNCTION(BlueprintCallable)
	//	void BeginPlay();
	//UFUNCTION(BlueprintCallable)
	//	void EndLevel();

	UFUNCTION(BlueprintCallable)
		void LoadLevel(FName levelName, bool useFade = true);

	UFUNCTION(BlueprintCallable)
		void LoadMenu(bool useFade = true, bool clearInput = true);

private:

	ARhythmOfTheDemonGameModeBase* m_RotDGameMode;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ATutorialHandler* m_tutoHandler;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ARailHandler* m_railHandler;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ABricksHandler* m_brickHandler;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ARhythmHandler* m_rhythmHandler;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ACameraHandler* m_cameraHandler;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AScoringHandler* m_scoringHandler;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName m_levelToLoad;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_isPause = false;

	int m_currentLevelIndex;

	void LevelEndRoutine();
	void StopTimeEffect();
	void PauseMusic();
	UFUNCTION(BlueprintCallable)
		void ResumeMusic();

	UFUNCTION()
		void InitLevel();
	void StartLevel();
	void SetupMenuLevel();

	bool m_isLevelEnded = false;
};
