// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "Enums/DifficulitesEnum.h"
#include "RhythmOfTheDemonGameModeBase.generated.h"

class APlayerEntity;
class AEnemyPawn;
class AMiniBossPawn;
class ACameraHandler;

/**
 *
 */
UCLASS()
class RHYTHMOFTHEDEMON_API ARhythmOfTheDemonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARhythmOfTheDemonGameModeBase();

	UFUNCTION(BlueprintCallable)
		APlayerEntity* GetPlayer();
	
	UFUNCTION(BlueprintCallable)
		AMiniBossPawn* GetEnemy();

	ACameraHandler*	GetCameraHandler() const { return m_cameraHandler; }
	
	UPROPERTY(EditAnywhere)
		UDataTable* LevelDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> PlayerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> AIActor;

	UFUNCTION(BlueprintCallable)
		void ClearCurrentLevel();

	UFUNCTION(BlueprintCallable)
		void ChangeCulture(const FString culture);

	void InstanciateEnemy(TSubclassOf<AActor> enemyActor);

	EDIFFICULTIES GetGameDifficulties() const { return m_gameDifficulties; }
	int GetOctaveNbFromCurrentDifficulty() const;


protected:
	virtual void BeginPlay() override;
	
private: 

	void InstanciatePlayer();

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		APlayerEntity* m_player;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ACameraHandler* m_cameraHandler;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName m_levelToLoadName;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EDIFFICULTIES m_gameDifficulties = EDIFFICULTIES::Normal;

	AMiniBossPawn* m_enemy;
};
