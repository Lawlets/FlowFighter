// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AbstractPlayerBody.h"

#include "ArmPlayer.generated.h"


/**
 *
 */
UCLASS()
class RHYTHMOFTHEDEMON_API AArmPlayer : public AAbstractPlayerBody
{
	GENERATED_BODY()

public:

	AArmPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//TODO: remove, only here to test control with 1 gamepad
	virtual void Switch() override;
	
	virtual void UnbindInput() override;
	virtual void BindInput() override;

protected:
	virtual void BeginPlay() override;

	virtual void RevertA() override;
	virtual void RevertB() override;
	virtual void RevertY() override;
	virtual void RevertX() override;
	virtual void Shoot() override;
	virtual void CastSoundWave() override;
	virtual void Aiming(float xAxis, float yAxis) override;

	virtual void InitController(TEnumAsByte<EAutoReceiveInput::Type> input) override;

	virtual void BindActionMapping() {}
	virtual void BindAxisMapping() {}

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SwitchPlayerControl() override;
};
