// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/InteractionResult.h"
#include "Enums/DifficulitesEnum.h"
#include "Enums/SuccessType.h"
#include "Enums/PlayerType.h"

#include "ScoringHandler.generated.h"

class AAbstractBrick;
class ARhythmOfTheDemonGameModeBase;
class UStreakComponnent;

UCLASS()
class RHYTHMOFTHEDEMON_API AScoringHandler : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AScoringHandler();
	static AScoringHandler* GetInstance() { return m_instance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void Init(EDIFFICULTIES difficulty);

	float GainScore(AAbstractBrick* brick);

	ESUCCESS_TYPE ComputeSuccessType(int currentTimelinePos, AAbstractBrick* brick);

	float ComputeDamageScore(AAbstractBrick* brick);
	float ComputeScore(float score, ESUCCESS_TYPE success);

	UFUNCTION(BlueprintCallable)
		float GetScore() const { return m_score; }

	UFUNCTION(BlueprintCallable)
		float GetScoreOfPlayer(int playerIndex);

	UFUNCTION(BlueprintCallable)
		void ResetScore();

protected:


	UPROPERTY(EditAnywhere)
		float m_perfectRequirement = 0.2f; 
	UPROPERTY(EditAnywhere)
		float m_coolRequirement = 0.5f;

	UPROPERTY(EditAnywhere)
		float m_perfectScoreMultiplier = 1.f;
	UPROPERTY(EditAnywhere)
		float m_coolScoreMultiplier = 0.5f;
	UPROPERTY(EditAnywhere)
		float m_okScoreMultiplier = 0.25f;

	UPROPERTY(EditAnywhere)
		float m_easyDifficultyDamageMultiplier = 0.5f;
	UPROPERTY(EditAnywhere)
		float m_mediumDifficultyDamageMultiplier = 1.f;
	UPROPERTY(EditAnywhere)
		float m_hardDifficultyDamageMultiplier = 2.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStreakComponnent* m_player1Streak;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStreakComponnent* m_player2Streak;

private:
	static AScoringHandler* m_instance;

	EDIFFICULTIES m_currentDifficulty;
	float m_score;

	float m_player1Score;
	float m_player2Score;
};
