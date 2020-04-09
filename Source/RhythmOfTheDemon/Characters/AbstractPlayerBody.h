// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enums/ActionMapping.h"
#include "AbstractPlayerBody.generated.h"

class APlayerEntity;
class ARhythmHandler;
class ARailHandler;
class ABricksHandler;

UCLASS()
class RHYTHMOFTHEDEMON_API AAbstractPlayerBody : public APawn
{
	GENERATED_BODY()

public:
	AAbstractPlayerBody();

	float	GetShootWindow() const { return m_shootWindow; }
	void	SetShootWindow(float value) { m_shootWindow = value; }

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float deltaTime) override;
	void UnbindAllActions();
	virtual void SwitchPlayerControl() {};
	
	virtual void UnbindInput() {};
	virtual void BindInput() {};

	void BindSpecificInput(EACTION_MAPPING Action);

	//TODO: remove, only here to test control with 1 gamepad
	virtual void Switch() {};
	void Pause();

protected:
	virtual void BeginPlay() override;

	virtual void RevertA();
	virtual void RevertX();
	virtual void RevertY();
	virtual void RevertB();
	virtual void Jump();
	virtual void Crouch();
	
	virtual void MoveUp(float value);
	virtual void MoveRight(float value);

	virtual void DashUp(float value);
	virtual void DashRight(float value);

	virtual void Shoot();
	virtual void CastSoundWave();
	virtual void Aiming(float xAxis, float yAxis);

	virtual void InitController(TEnumAsByte<EAutoReceiveInput::Type> input) {};

	virtual void BindActionMapping() {}
	virtual void BindAxisMapping() {}

	void BindArmPlayerAction(UInputComponent* PlayerInputComponent);
	void BindLegPlayerAction(UInputComponent* PlayerInputComponent);

	void UnbinAllInputOnController(UInputComponent* PlayerInputComponent);
	void UnbindAllActionsOnController(UInputComponent* PlayerInputComponent);
	void UnbindAllAxisOnController(UInputComponent* PlayerInputComponent);

private:
	void BindCommonPlayerAction(UInputComponent* PlayerInputComponent);

public:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
		APlayerEntity* m_owner = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EAutoReceiveInput::Type> m_controllerInput;

	ARhythmHandler* rhythmHdl = nullptr;
	ABricksHandler* brickHdl;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_shootWindow;
};
