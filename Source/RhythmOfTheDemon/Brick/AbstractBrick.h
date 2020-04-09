// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Enums/ActionMapping.h"
#include "Enums/InteractionResult.h"
#include "Enums/BrickState.h"
#include "Enums/PlayerType.h"
#include "Enums/SuccessType.h"

#include "Actors/Components/DissolveComponent.h"
#include "Animation/BrickAnimInstance.h"

#include "AbstractBrick.generated.h"

class AAbstractEntityActor;
class UPaperSpriteComponent;

class ARhythmHandler;
class ABricksHandler;
class AGameplaySpaceAnchor;

class AAbstractBrick; // Enable tevent with abstract brick as parameter


DECLARE_EVENT_OneParam(AAbstractBrick, BrickInteraction, AAbstractBrick*);
DECLARE_EVENT_OneParam(AAbstractBrick, PerfectTimingEvent, AAbstractBrick*);

DECLARE_EVENT_OneParam(AAbstractBrick, IdleStateEvent, AAbstractBrick*);
DECLARE_EVENT_OneParam(AAbstractBrick, PreparationStateEvent, AAbstractBrick*);
DECLARE_EVENT_OneParam(AAbstractBrick, HitStateEvent, AAbstractBrick*);
DECLARE_EVENT_OneParam(AAbstractBrick, PassedStateEvent, AAbstractBrick*);

UCLASS()
class RHYTHMOFTHEDEMON_API AAbstractBrick : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbstractBrick(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target);
	virtual void Tick(float DeltaTime) override;
	virtual void Update(int timelinePosition);

	virtual EINTERACTION_RESULT Interact(float timelinePos, bool isFromArmPlayer);

	EACTION_MAPPING GetType() const					{ return type; }
	EBRICK_STATE GetState() const					{ return state; }
	EINTERACTION_RESULT GetInteractionState() const { return m_interactionState; }
	EPLAYER_TYPE GetTargetType() const				{ return m_targetPlayerType; }
	ESUCCESS_TYPE GetSuccessType() const			{ return m_successType; }

	int GetIdleWindow() const			{ return m_travelTime - errorWindow; }
	int GetActivationWindow() const		{ return activationWindow; }
	int GetErrorWindow() const			{ return errorWindow; }
	int GetTimelinePosition() const		{ return m_timelinePosition; }
	float GetScoreValue() const			{ return scoreValue; }
	float GetDynamicScoreValue() const	{ return m_dynamicScoreValue; }

	bool IsInUse() const			{ return m_inUse; }
	bool IsInteractable() const		{ return m_interactionState == EINTERACTION_RESULT::None; }

	void SetTimelinePosition(int value)						{ m_timelinePosition = value; }
	virtual void SetSuccessType(ESUCCESS_TYPE success)		{ m_successType = success; }
	void SetDynamicScoreValue(float newScore)				{ m_dynamicScoreValue = newScore; }

	// Should be in a parent class specific to AActorPoolObject
	virtual void Show();
	virtual void Hide();

	void UpdateState(int timelinePos);
	virtual void SwitchState(EBRICK_STATE newState);
	virtual void PerfectTiming();

	// Event
	BrickInteraction& OnBrickInteraction() { return m_brickInteraction; }
	PerfectTimingEvent& OnPerfectTimingEvent() { return m_perfectTimingEvent; }

	IdleStateEvent& OnIdleStateEvent() { return m_idleStateEvent; }
	PreparationStateEvent& OnPreparationStateEvent() { return m_preparationStateEvent; }
	HitStateEvent& OnHitStateEvent() { return m_hitStateEvent; }
	PassedStateEvent& OnPassedStateEvent() { return m_passedStateEvent; }

protected:

	void BroadcastBrickInteraction();

	UMaterialInstanceDynamic* m_dynamicMaterial;
	UBrickAnimInstance* m_animInstance;

	// Should be in a parent class specific to AActorPoolObject
	bool m_inUse = false;

	ABricksHandler* m_brickHdl;
	AGameplaySpaceAnchor* m_gameplaySpaceAnchor;

	//Use caster & target instead ??
	AAbstractEntityActor* m_player;
	AAbstractEntityActor* m_enemy;
	int m_timelinePosition;
	int m_creationTimelinePosition;

	int m_travelTime;
	bool m_isPerfectTimingPassed = false;
	float m_dynamicScoreValue = 0.f;

	EBRICK_STATE state;
	EINTERACTION_RESULT m_interactionState = EINTERACTION_RESULT::None;
	ESUCCESS_TYPE m_successType;

	// GamePlay
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		EACTION_MAPPING type;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		EPLAYER_TYPE m_targetPlayerType;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		int activationWindow = 200;
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		int errorWindow = 700;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float scoreValue = 10;

	// Graphics
	UPROPERTY(EditAnywhere, Category = "Graphics")
		FLinearColor m_idleColor = FLinearColor(1, 1, 1);
	UPROPERTY(EditAnywhere, Category = "Graphics")
		float m_idleEmissiveForce = 10;

	UPROPERTY(EditAnywhere, Category = "Graphics")
		FLinearColor m_prepColor = FLinearColor(1, 1, 1);
	UPROPERTY(EditAnywhere, Category = "Graphics")
		float m_prepEmissiveForce = 10;

	UPROPERTY(EditAnywhere, Category = "Graphics")
		FLinearColor m_hitColor = FLinearColor(1, 1, 1);
	UPROPERTY(EditAnywhere, Category = "Graphics")
		float m_hitEmissiveForce = 10;

	UPROPERTY(EditAnywhere, Category = "Graphics")
		float m_appearDuration = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Graphics")
		float m_disappearDuration = 1.f;

	// COMPONENTS
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USceneComponent* m_root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UDissolveComponent* m_dissolveComponent;

private:
	// Event
	BrickInteraction m_brickInteraction;
	PerfectTimingEvent m_perfectTimingEvent;

	IdleStateEvent m_idleStateEvent;
	PreparationStateEvent m_preparationStateEvent;
	HitStateEvent m_hitStateEvent;
	PassedStateEvent m_passedStateEvent;
};
