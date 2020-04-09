// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractPlayerBody.h"

#include "Sound/RhythmHandler.h"
#include "Brick/BricksHandler.h"
#include "RhythmOfTheDemonGameStateBase.h"

#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

#include "RhythmOfTheDemonGameModeBase.h"

AAbstractPlayerBody::AAbstractPlayerBody()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAbstractPlayerBody::BeginPlay()
{
	Super::BeginPlay();

	rhythmHdl = ARhythmHandler::GetInstance();
	brickHdl = ABricksHandler::GetInstance();
}

void AAbstractPlayerBody::RevertA()
{
	UE_LOG(LogTemp, Log, TEXT("REVERT_A from AbstractPlayerBody"));
}

void AAbstractPlayerBody::RevertX()
{
	UE_LOG(LogTemp, Log, TEXT("REVERT_X from AbstractPlayerBody"));
}

void AAbstractPlayerBody::RevertY()
{
	UE_LOG(LogTemp, Log, TEXT("REVERT_Y from AbstractPlayerBody"));
}

void AAbstractPlayerBody::RevertB()
{
	UE_LOG(LogTemp, Log, TEXT("REVERT_B from AbstractPlayerBody"));
}

void AAbstractPlayerBody::Jump()
{
	UE_LOG(LogTemp, Log, TEXT("JUMP from AbstractPlayerBody"));
}

void AAbstractPlayerBody::Crouch()
{
	UE_LOG(LogTemp, Log, TEXT("CROUCH from AbstractPlayerBody"));
}

void AAbstractPlayerBody::MoveUp(float value)
{
	UE_LOG(LogTemp, Log, TEXT("MOVE_UP from AbstractPlayerBody"));
}

void AAbstractPlayerBody::MoveRight(float value)
{
	UE_LOG(LogTemp, Log, TEXT("MOVE_RIGHT from AbstractPlayerBody"));
}

void AAbstractPlayerBody::DashUp(float value)
{
	UE_LOG(LogTemp, Log, TEXT("DASH_UP from AbstractPlayerBody"));
}

void AAbstractPlayerBody::DashRight(float value)
{
	UE_LOG(LogTemp, Log, TEXT("DASH_RIGHT from AbstractPlayerBody"));
}

void AAbstractPlayerBody::Shoot()
{
	UE_LOG(LogTemp, Log, TEXT("SHOOT from AbstractPlayerBody"));
}

void AAbstractPlayerBody::CastSoundWave()
{
	/*if (brickHdl != nullptr)
		brickHdl->Interact(EACTION_MAPPING::CastSoundWave);*/
	UE_LOG(LogTemp, Log, TEXT("CAST_SOUND_WAVE from AbstractPlayerBody"));
}

void AAbstractPlayerBody::Aiming(float xAxis, float yAxis)
{
}

void AAbstractPlayerBody::BindArmPlayerAction(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("BindArmPlayerAction in AbstractPlayerBody"));

	PlayerInputComponent->BindAction("RevertA", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::RevertA);
	//PlayerInputComponent->BindAction("RevertX", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::RevertX);
	//PlayerInputComponent->BindAction("RevertY", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::RevertY);
	PlayerInputComponent->BindAction("RevertB", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::RevertB);

	BindCommonPlayerAction(PlayerInputComponent);
}

void AAbstractPlayerBody::BindLegPlayerAction(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("BindLegPlayerAction in AbstractPlayerBody"));

	//PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::Jump);
	//PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::Crouch);

	PlayerInputComponent->BindAxis("Forward", this, &AAbstractPlayerBody::MoveUp);
	PlayerInputComponent->BindAxis("Right", this, &AAbstractPlayerBody::MoveRight);

	PlayerInputComponent->BindAxis("DashRight", this, &AAbstractPlayerBody::DashRight);
	PlayerInputComponent->BindAxis("DashUp", this, &AAbstractPlayerBody::DashUp);

	BindCommonPlayerAction(PlayerInputComponent);
}

void AAbstractPlayerBody::UnbinAllInputOnController(UInputComponent* PlayerInputComponent)
{
	UnbindAllActionsOnController(PlayerInputComponent);
	UnbindAllAxisOnController(PlayerInputComponent);
}

void AAbstractPlayerBody::UnbindAllActionsOnController(UInputComponent * PlayerInputComponent)
{
	PlayerInputComponent->ClearActionBindings();
}

void AAbstractPlayerBody::UnbindAllAxisOnController(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->AxisBindings.Empty();
}

void AAbstractPlayerBody::BindSpecificInput(EACTION_MAPPING Action)
{
	const int32 PlayerIndex = int32(AutoReceiveInput) - 1;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	UInputComponent* PlayerInputComponent = PC->InputComponent;

	UnbinAllInputOnController(PlayerInputComponent);

	switch (Action)
	{
	case EACTION_MAPPING::RevertA:
		PlayerInputComponent->BindAction("RevertA", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::RevertA);
		break;

	case EACTION_MAPPING::RevertB:
		PlayerInputComponent->BindAction("RevertB", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::RevertB);
		break;

	case EACTION_MAPPING::DashVertical:
		PlayerInputComponent->BindAxis("DashUp", this, &AAbstractPlayerBody::DashUp);
		break;

	case EACTION_MAPPING::DashHorizontal:
		PlayerInputComponent->BindAxis("DashRight", this, &AAbstractPlayerBody::DashRight);
		break;

	default:
		break;
	}

	BindCommonPlayerAction(PlayerInputComponent);
}

void AAbstractPlayerBody::UnbindAllActions()
{
	APlayerController* PC_0 = UGameplayStatics::GetPlayerController(this, EAutoReceiveInput::Player0 - 1);
	if (PC_0)
	{
		UnbindAllActionsOnController(PC_0->InputComponent);
		UnbindAllAxisOnController(PC_0->InputComponent);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("PC_0 is null, cannot unbind all action"));
	
	APlayerController* PC_1 = UGameplayStatics::GetPlayerController(this, EAutoReceiveInput::Player1 - 1);
	if (PC_1)
	{
		UnbindAllActionsOnController(PC_1->InputComponent);
		UnbindAllAxisOnController(PC_1->InputComponent);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("PC_1 is null, cannot unbind all action"));

}

void AAbstractPlayerBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbstractPlayerBody::BindCommonPlayerAction(UInputComponent * PlayerInputComponent)
{
	PlayerInputComponent->BindAction("CastSoundWave", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::CastSoundWave);
	//PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::Shoot);
	PlayerInputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::Pause);

	PlayerInputComponent->BindAction("Switch", EInputEvent::IE_Pressed, this, &AAbstractPlayerBody::Switch);
}

void AAbstractPlayerBody::Pause()
{
	ARhythmOfTheDemonGameStateBase* gameState = GetWorld()->GetGameState<ARhythmOfTheDemonGameStateBase>();
	if (gameState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get ARhythmOfTheDemonGameStateBase in ARailHandler::OnRailEnd"));
		return;
	}
	gameState->PauseGame();
}

void AAbstractPlayerBody::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
}

