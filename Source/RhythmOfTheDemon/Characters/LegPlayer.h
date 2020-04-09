// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AbstractPlayerBody.h"

#include "Engine/World.h"
#include "LegPlayer.generated.h"

/**
 *
 */
UCLASS()
class RHYTHMOFTHEDEMON_API ALegPlayer : public AAbstractPlayerBody
{
	GENERATED_BODY()

public:

	ALegPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Switch() override;

	bool CheckMovementDelimitation(FVector position, FVector& correctedPos);
	virtual void UnbindInput() override;
	virtual void BindInput() override;

protected:
	virtual void BeginPlay() override;

	virtual void Jump() override;
	virtual void Crouch() override;

	virtual void MoveUp(float value) override;
	virtual void MoveRight(float value) override;
	bool Dash(float value, FVector inputDir);

	virtual void DashUp(float value) override;
	virtual void DashRight(float value) override;

	virtual void Shoot() override;
	virtual void CastSoundWave() override;
	virtual void Aiming(float xAxis, float yAxis) override;

	virtual void InitController(TEnumAsByte<EAutoReceiveInput::Type> input) override;

	virtual void BindActionMapping() {}
	virtual void BindAxisMapping() {}

private:
	FVector ComputeMovement(FVector direction);
	FVector ComputeDash(FVector direction, FVector inputDir);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector2D m_horizontalDelimitiaton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector2D m_verticalDelimitation;

	bool m_hasRecenterUpStick = true;
	bool m_hasRecenterRightStick = true;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SwitchPlayerControl() override;

	AWorldSettings* m_worldSettings;
};
