// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbstractBrick.h"
#include "DataStruct/BrickData.h"
#include "DataStruct/LevelDataStruct.h"
#include "MidiBricksTranslator.h"
#include "Sound/Midi/Data/EventEnum.h"
#include "FMODEvent.h"

#include "BricksHandler.generated.h"


class ARailHandler;
class ARhythmHandler;
class AFeedbackHandler;

class APlayerEntity;
class AEnemyPawn;
class AMiniBossPawn;
class AScoringHandler;


DECLARE_EVENT_OneParam(ABricksHandler, BrickInstantiationEvent, AAbstractBrick*);

UCLASS()
class RHYTHMOFTHEDEMON_API ABricksHandler : public AActor
{
	GENERATED_BODY()

private:
	static ABricksHandler* m_instance;

public:
	// Sets default values for this actor's properties
	ABricksHandler();

	static ABricksHandler* GetInstance() { return m_instance; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(APlayerEntity* player, AMiniBossPawn* enemy, FLevelDataStruct* levelData);
	void ClearBricks();

	void Interact(EACTION_MAPPING action, bool isFromArmPlayer = true);

	int GetLevelProjectileCount() { return midiTranslator.GetProjectileCount(); }
	int GetLevelCoopCount() { return midiTranslator.GetCoopCount(); }

	BrickInstantiationEvent& OnBrickInstantiation() { return m_brickInstantiationEvent; }

protected:

	UPROPERTY(EditAnywhere)
		bool DisableBricks = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
		UForceFeedbackEffect* m_onErrorControllerShake;

private:
	void InitLevelObstacles(FLevelDataStruct* levelData);

	void CheckForBrickInstantiation(int timelinePos);
	void InstantiateObstacle(FBrickDataStruct brickData);
	
	void UpdateObstaclesFromArray(TArray<AAbstractBrick*> obstacleArray, float currTimelinePosition);
	void ComputeBrickRepercussionFromArray(TArray<AAbstractBrick*>& bricks);
	void RemoveUnusedBricks(TArray<AAbstractBrick*>& bricks);

	//Should be in pool
	AAbstractBrick* FindUnusedBrick(TSubclassOf<AAbstractBrick> subclass);

	//Feedback => May be mooved out of BrickHandler
	void PositiveFeedback(AAbstractBrick* brick);
	void NegativeFeedback(AAbstractBrick* brick);
	void NoInteractionFeedback(AAbstractBrick* brick);

	void ComputeBrickRepercussion(AAbstractBrick* brick);
	void ComputeGoodInteraction(AAbstractBrick* brick);
	void ComputeBadInteraction(AAbstractBrick* brick);

	//void NegateInteractionFromBrick(AAbstractBrick* brick, TArray<AAbstractBrick*>& brickContainer);

	ARhythmHandler* m_rhythmHdl;
	AFeedbackHandler* m_feedbackHdl;
	AScoringHandler* m_scoringHandler;

	APlayerEntity* m_player;
	AMiniBossPawn* m_enemy;

	MidiBricksTranslator midiTranslator;

	TArray<FBrickDataStruct> m_futureObstacle;
	TArray<AAbstractBrick*> obstaclePool;

	TArray<TArray<AAbstractBrick*>> m_brickArrayByType;

	//Event
	BrickInstantiationEvent m_brickInstantiationEvent;

	UPROPERTY(EditAnywhere)
		int brickTravelDuration = 3000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<AAbstractBrick>> typeArray;

	UPROPERTY(EditAnywhere)
		int poolSizePerBricks = 15;

	UPROPERTY(EditAnywhere)
		TMap<Note, TSubclassOf<AAbstractBrick>> MidiBrickMap;

	const int MANAGED_BRICK_ID = (int)EACTION_MAPPING::None;
};
