// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Brick/AbstractBrick.h"

#include "TutorialHandler.generated.h"

class ABricksHandler;
class ARhythmHandler;
class AFeedbackHandler;

class APlayerEntity;

UCLASS()
class RHYTHMOFTHEDEMON_API ATutorialHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialHandler();
	static ATutorialHandler* GetInstance() { return m_instance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	static ATutorialHandler* m_instance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();
	bool IsTutorialDone();
	void EndTutorial();

	void ManageTutorialBrick(AAbstractBrick* brick);

	UFUNCTION(BlueprintCallable)
		bool IsTutorialFreezingTime() { return m_isTutorialFreezingTime; }

protected:
	UFUNCTION(BlueprintCallable)
		void ChangeActorTimeDilatationTo(AActor* actor, float value);
	
	UFUNCTION(BlueprintImplementableEvent)
		void ShowTutoStepFeedback(int stepIdx, float duration);

	UFUNCTION(BlueprintImplementableEvent)
		void ShowInputUI(EACTION_MAPPING action);

	UFUNCTION(BlueprintImplementableEvent)
		void HideInputUI();

	UFUNCTION(BlueprintImplementableEvent)
		void SlowActorTime(AActor* actor, float duration);

	UFUNCTION(BlueprintImplementableEvent)
		void UnSlowActorTime(AActor* actor, float duration);

	UPROPERTY(EditAnywhere, Category="Tutorial")
		int m_BrickCountToCheck = 5;

	UPROPERTY(EditAnywhere, Category = "Feedback")
		TArray<int> m_tutoStepTiming;

	UPROPERTY(EditAnywhere, Category = "Feedback")
		float m_stepTextDuration = 3.f;

private:

	void BeginTutoPhase(AAbstractBrick* brick);
	void ShowCurrentInput(AAbstractBrick* brick);
	void FreezePhase(AAbstractBrick* brick);
	void EndTutoPhase(AAbstractBrick* brick);

	void LockInput(EPLAYER_TYPE target, EACTION_MAPPING action);
	void UnlockInput(EPLAYER_TYPE target);

	ABricksHandler* m_bricksHdl;
	ARhythmHandler* m_rhythmHdl;
	AFeedbackHandler* m_feedbackHdl;

	APlayerEntity* m_player;

	TArray<AAbstractBrick*> m_tutoBrickArray;

	int tutoPhaseCount = 0;
	int m_currTutoStepIdx = 0;
	bool m_isTutorialFreezingTime = false;
};
