// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmPlayer.h"
#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "Runtime/Engine/Public/EngineUtils.h"

#include "RhythmOfTheDemon.h"
#include "Engine.h"

#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Actors/PlayerEntity.h"

AArmPlayer::AArmPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AArmPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent in ArmPlayer"));

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AArmPlayer::Switch()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, TEXT("Switching to Legs !"));

	if (m_owner != nullptr)
		m_owner->SwitchControl();
	else
		UE_LOG(LogTemp, Warning, TEXT("Null reference of owner in Switch of ArmPlayer"));
}

void AArmPlayer::BeginPlay()
{
	Super::BeginPlay();
	InitController(EAutoReceiveInput::Player0);
	
	//UnbindInput();
	//BindInput();
}

void AArmPlayer::UnbindInput()
{
	const int32 PlayerIndex = int32(AutoReceiveInput) - 1;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	UnbinAllInputOnController(PC->InputComponent);
}

void AArmPlayer::BindInput()
{
	const int32 PlayerIndex = int32(AutoReceiveInput) - 1;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	BindArmPlayerAction(PC->InputComponent);
	//FInputModeGameOnly inputMode = FInputModeGameOnly::FInputModeGameOnly();
	//PC->SetInputMode(inputMode);
}

void AArmPlayer::Shoot()
{
	//Super::Shoot();
	/*if(brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::Shoot);*/
	UE_LOG(LogTemp, Log, TEXT("SHOOT from ArmPlayer"));
}

void AArmPlayer::RevertA()
{
	if (m_owner != nullptr)
		m_owner->RevertA();

	if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::RevertA);
	UE_LOG(LogTemp, Log, TEXT("REVERT_A from ArmPlayer"));
}

void AArmPlayer::RevertB()
{
	if (m_owner != nullptr)
		m_owner->RevertB();

	if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::RevertB);
	UE_LOG(LogTemp, Log, TEXT("REVERT_B from ArmPlayer"));
}

void AArmPlayer::RevertY()
{
	/*if (m_owner != nullptr)
		m_owner->RevertY();

	if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::RevertY);*/
	UE_LOG(LogTemp, Log, TEXT("REVERT_Y from ArmPlayer"));
}

void AArmPlayer::RevertX()
{
	/*if (m_owner != nullptr)
		m_owner->RevertX();

	if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::RevertX);*/
	UE_LOG(LogTemp, Log, TEXT("REVERT_X from ArmPlayer"));
}

void AArmPlayer::CastSoundWave()
{
	//Super::CastSoundWave();
	m_owner->ShockWave();
	if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::CastSoundWave, true);
	UE_LOG(LogTemp, Log, TEXT("CAST_SOUND_WAVE from ArmPlayer"));
}

void AArmPlayer::Aiming(float xAxis, float yAxis)
{
	Super::Aiming(xAxis, yAxis);
}

void AArmPlayer::InitController(TEnumAsByte<EAutoReceiveInput::Type> input)
{
	UE_LOG(LogTemp, Warning, TEXT("InitController in ArmPlayer"));

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PC is null"));
	}
}


void AArmPlayer::SwitchPlayerControl()
{
	if (m_controllerInput.GetValue() == EAutoReceiveInput::Player0)
		InitController(EAutoReceiveInput::Player1);
	else
		InitController(EAutoReceiveInput::Player0);

	BindArmPlayerAction(Controller->InputComponent);
}

void AArmPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


