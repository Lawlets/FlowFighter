// Fill out your copyright notice in the Description page of Project Settings.

#include "LegPlayer.h"
#include "Engine.h"
#include "Enums/DashType.h"

#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "Actors/RailHandler.h"
#include "Actors/CameraHandler.h"
#include "Kismet/KismetMathLibrary.h"


#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/PlayerEntity.h"

ALegPlayer::ALegPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALegPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALegPlayer::Switch()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, TEXT("Switching to Arms !"));

	if (m_owner != nullptr)
		m_owner->SwitchControl();
	else
		UE_LOG(LogTemp, Warning, TEXT("Null reference of owner in Switch of ArmPlayer"));
}

void ALegPlayer::BeginPlay()
{
	Super::BeginPlay();

	m_horizontalDelimitiaton = FVector2D(-15, 14);
	m_verticalDelimitation = FVector2D(-10, 14);
	m_worldSettings = GetWorld()->GetWorldSettings();

	InitController(EAutoReceiveInput::Player1);
}

void ALegPlayer::UnbindInput()
{
	const int32 PlayerIndex = int32(AutoReceiveInput) - 1;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	UnbinAllInputOnController(PC->InputComponent);
}

void ALegPlayer::BindInput()
{
	const int32 PlayerIndex = int32(AutoReceiveInput) - 1;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	BindLegPlayerAction(PC->InputComponent);
	//FInputModeGameOnly inputMode = FInputModeGameOnly::FInputModeGameOnly();
	//PC->SetInputMode(inputMode);
}

void ALegPlayer::Jump()
{
}

void ALegPlayer::Crouch()
{
	/*if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::Crouch);*/
	UE_LOG(LogTemp, Log, TEXT("CROUCH from LegPlayer"));
}

bool ALegPlayer::Dash(float value, FVector inputDir)
{
	if (value == 0.f)
		return false;

	const int32 PlayerIndex = int32(AutoReceiveInput) - 1;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	float axisRightValue = PC->PlayerInput->GetKeyValue(EKeys::Gamepad_RightX);
	float axisUpValue = PC->PlayerInput->GetKeyValue(EKeys::Gamepad_RightY) * -1.0f;

	FVector direction = (axisRightValue * FVector::RightVector) + (axisUpValue * FVector::UpVector);
	FVector newPos = ComputeDash(direction, inputDir);
	FVector correctedPos;

	bool canDash = CheckMovementDelimitation(newPos, correctedPos);

	EDASH_TYPE type = EDASH_TYPE::None;

	if (canDash)
		type = m_owner->Dash(newPos, direction);
	else
		type = m_owner->Dash(correctedPos, direction);

	return (type != EDASH_TYPE::None);
}

void ALegPlayer::MoveRight(float value)
{
	if (value == 0)
	{
		FVector location = m_owner->GetSkeletalMeshComponent()->RelativeLocation;
		m_owner->MoveRight(location, value);
		return;
	}

	FVector newPos = ComputeMovement(FVector::RightVector * value);
	m_owner->MoveRight(newPos, value);
}

void ALegPlayer::MoveUp(float value)
{
	if (value == 0)
	{
		FVector location = m_owner->GetSkeletalMeshComponent()->RelativeLocation;
		m_owner->MoveUp(location, value);
		return;
	}

	FVector newPos = ComputeMovement(FVector::UpVector * value);
	m_owner->MoveUp(newPos, value);
}

void ALegPlayer::DashUp(float value)
{
	if (value == 0.f)
		m_hasRecenterUpStick = true;

	if ((value >= -0.25f && value <= 0.25f )|| m_owner->IsDashing())
		return;

	if (!m_hasRecenterUpStick  || !m_hasRecenterRightStick)
		return;

	if (Dash(value, FVector::UpVector * value))
	{
		m_hasRecenterUpStick = false;
		UE_LOG(LogTemp, Log, TEXT(" | Dash from LegPlayer"));
		brickHdl->Interact(EACTION_MAPPING::DashVertical);
	}
}

void ALegPlayer::DashRight(float value)
{
	if (value == 0.f)
		m_hasRecenterRightStick = true;

	if ((value >= -0.25f && value <= 0.25f) || m_owner->IsDashing())
		return;

	if (!m_hasRecenterRightStick || !m_hasRecenterUpStick)
		return;

	if (Dash(value, FVector::RightVector * value))
	{
		m_hasRecenterRightStick = false;
		UE_LOG(LogTemp, Log, TEXT(" --- Dash from LegPlayer"));
		brickHdl->Interact(EACTION_MAPPING::DashHorizontal);
	}
}

FVector ALegPlayer::ComputeMovement(FVector direction)
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;

	FVector meshPos = m_owner->GetSkeletalMeshComponent()->RelativeLocation;
	direction *= m_owner->m_movementSpeed * deltaTime;

	FVector newPos = meshPos + direction;
	FVector correctedPos;
	if (CheckMovementDelimitation(newPos, correctedPos) == false)
		return correctedPos;

	return newPos;
}

FVector ALegPlayer::ComputeDash(FVector direction, FVector inputDir)
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	float dashLength = 0.f;

	if (inputDir == FVector::RightVector || inputDir == -FVector::RightVector)
		dashLength = m_owner->m_horizontalDashLength;
	else
		dashLength = m_owner->m_verticalDashLength;


	FVector meshPos = m_owner->GetSkeletalMeshComponent()->RelativeLocation;
	direction.Normalize();
	direction *= dashLength;
	direction *= m_owner->m_movementSpeed * deltaTime;
	return meshPos + direction;
}

bool ALegPlayer::CheckMovementDelimitation(FVector position, FVector& correctedPos)
{
	bool result = true;
	correctedPos = position;

	if (position.Y < m_horizontalDelimitiaton.X)
	{
		result = false;
		correctedPos.Y = m_horizontalDelimitiaton.X;
	}

	if (position.Y > m_horizontalDelimitiaton.Y)
	{
		result = false;
		correctedPos.Y = m_horizontalDelimitiaton.Y;
	}

	if (position.Z < m_verticalDelimitation.X)
	{
		result = false;
		correctedPos.Z = m_verticalDelimitation.X;
	}

	if (position.Z > m_verticalDelimitation.Y)
	{
		result = false;
		correctedPos.Z = m_verticalDelimitation.Y;
	}

	return result;
}

void ALegPlayer::Shoot()
{
	/*if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::Shoot);*/
		//Super::Shoot();
	UE_LOG(LogTemp, Log, TEXT("SHOOT from LegPlayer"));
}

void ALegPlayer::CastSoundWave()
{
	//Super::CastSoundWave();
	m_owner->ShockWave();
	if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::CastSoundWave, false);
	UE_LOG(LogTemp, Log, TEXT("CAST_SOUND_WAVE from LegPlayer"));
}

void ALegPlayer::Aiming(float xAxis, float yAxis)
{
	Super::Aiming(xAxis, yAxis);
}

void ALegPlayer::InitController(TEnumAsByte<EAutoReceiveInput::Type> input)
{
	UE_LOG(LogTemp, Warning, TEXT("InitController in LegPlayer"));

	Super::InitController(input);

	m_controllerInput = input;
	AutoReceiveInput = input;
	const int32 PlayerIndex = int32(AutoReceiveInput) - 1;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if (PC)
	{
		PC->bAutoManageActiveCameraTarget = false;
		PC->Possess(this);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PC is null"));
	}
}

void ALegPlayer::SwitchPlayerControl()
{
	if (m_controllerInput.GetValue() == EAutoReceiveInput::Player0)
		InitController(EAutoReceiveInput::Player1);
	else
		InitController(EAutoReceiveInput::Player0);

	BindLegPlayerAction(Controller->InputComponent);
}

void ALegPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
