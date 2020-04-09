// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"

#include "Actors/AbstractEntityActor.h"
#include "Enums/DashType.h"
#include "Enums/PlayerLifeState.h"
#include "Enums/InteractionResult.h"
#include "Actors/ChildActor/CameraChildActorComponent.h"

#include "PlayerEntity.generated.h"

class ALegPlayer;
class AArmPlayer;
class USpringArmComponent;
class UPaperSpriteComponent;
class UCurveFloat;
class USplineComponent;
//class UCameraChildActorComponent;

class AFeedbackHandler;
class ARhythmHandler;
class AGameplaySpaceAnchor;
class ARail;

/**
 *
 */
UCLASS()
class RHYTHMOFTHEDEMON_API APlayerEntity : public AAbstractEntityActor
{
	GENERATED_BODY()

public:
	APlayerEntity(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRegenerate_Event);

	void Init(ARail* newRail, UCurveFloat* newSpeedCurve);
	void Reset();

	virtual void Tick(float DeltaTime) override;
	void SwitchControl();

	void MoveRight(FVector newPos, float rightValue);
	void MoveUp(FVector newPos, float upValue);

	EDASH_TYPE Dash(FVector DashPoint, FVector DashDirection);
	void ShockWave();

	FVector GetDirection() const { return FVector(0.f, m_yaw, m_pitch); }
	float GetCurrentSplineDistance() override { return m_currentSplineDistance; }

	UFUNCTION(BlueprintCallable)
	bool IsDashing() const { return m_isDashing; }

	void ClearPlayerInput();
	void SetPlayerInput();

	virtual float TakeDamage(float amount, const FDamageEvent& dmgEvent, AController* ctrlr, AActor* actor);
	void Regenerate(float amount);

	void ResetSplineDistance();

	UPROPERTY(BlueprintAssignable)
		FRegenerate_Event OnRegenerate;

	UFUNCTION(BlueprintCallable)
		EPLAYER_LIFE_STATE GetPlayerLifeState() const { return m_playerLifeState; }

	UFUNCTION(BlueprintCallable)
		AActor* GetQuarterRightCameraChildActor() const { return m_quarterRightCameraActorComponent->GetChildActor(); }
	UFUNCTION(BlueprintCallable)
		AActor* GetQuarterLeftCameraChildActor() const { return m_quarterLeftCameraActorComponent->GetChildActor(); }
	UFUNCTION(BlueprintCallable)
		AActor* GetPauseCameraChildActor() const { return m_pauseCameraActorComponent->GetChildActor(); }
	UFUNCTION(BlueprintCallable)
		AActor* GetMenuCameraChildActor() const { return m_menuCameraActorComponent->GetChildActor(); }

	UFUNCTION(BlueprintCallable)
		ALegPlayer* GetLegPlayerPawn() const { return m_legPlayer; }

	UFUNCTION(BlueprintCallable)
		AArmPlayer* GetArmPlayerPawn() const { return m_armPlayer; }

protected:

	virtual void BeginPlay() override;
	virtual bool IsDead() override;

	virtual void EntityDie() override;


private:
	void UpdateMovementFromSpline(float deltaTime);
	void ComputeDashMovement();
	void ComputeLifeState();
	void ComputeMovementLerp(float tick);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_movementSpeed = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_speed = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_yaw = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_pitch = 0.f;

	UPROPERTY(EditAnywhere, Category = "Dash")
		float m_verticalDashLength = 50.f;
	UPROPERTY(EditAnywhere, Category = "Dash")
		float m_horizontalDashLength = 50.f;

	void RevertA();
	void RevertB();
	void RevertY();
	void RevertX();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
		TSubclassOf<UCameraShake> m_onDamageTakenCameraShake;

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void RevertA_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void RevertB_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void RevertY_Event();
	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void RevertX_Event();

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void Dash_Event(FVector direction);

	//Dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		float m_dashSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Dash")
		float m_dashCooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		FVector n_meshDashStartPos = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		UFMODEvent* m_dashSound;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_quarterRightSpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_quarterLeftSpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_pauseCameraSpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_menuCameraSpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraChildActorComponent* m_quarterRightCameraActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraChildActorComponent* m_quarterLeftCameraActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraChildActorComponent* m_pauseCameraActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraChildActorComponent* m_menuCameraActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AArmPlayer* m_armPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ALegPlayer* m_legPlayer;

	
	float m_currentSplineDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPLAYER_LIFE_STATE m_playerLifeState;


	bool m_isDashing = false;
	bool m_canDash = true;
	float m_dashTimer = 0.f;
	FVector m_dashFinalPoint = FVector::ZeroVector;
	float m_dashLerpDeltaTime = 0.f;

	AGameplaySpaceAnchor* m_gameplaySpaceAnchor;

	AFeedbackHandler* m_feedbackHdl;
	ARhythmHandler* m_rhythmHdl;
	UCurveFloat* m_speedCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_yawLerpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_pitchLerpSpeed;

	float m_yawTargetValue;
	float m_yawDeltaLerp;

	float m_pitchTargetValue;
	float m_pitchDeltaLerp;

};
