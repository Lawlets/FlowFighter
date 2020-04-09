// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerEntity.h"
#include "Characters/ArmPlayer.h"
#include "Characters/LegPlayer.h"
#include "Actors/RailHandler.h"

#include "Actors/FeedbackHandler.h"
#include "Actors/GameplaySpaceAnchor.h"
#include "Actors/CameraHandler.h"
#include "Sound/RhythmHandler.h"
#include "RhythmOfTheDemonGameStateBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "PaperSpriteComponent.h"

#include "Actors/Components/StreakComponnent.h"
#include "Actors/Components/RandomMovementComponent.h"
//#include "Actors/ChildActor/CameraChildActorComponent.h"

#include "Curves/RichCurve.h"
#include "Engine/World.h"
#include "Engine.h"
#include "EngineUtils.h"

#include "Misc/OutputDeviceNull.h"

#include "Math/UnrealMathUtility.h"

#include "FMODBlueprintStatics.h"

APlayerEntity::APlayerEntity(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	m_quarterRightSpringArm = CreateOptionalDefaultSubobject<USpringArmComponent>(TEXT("QuarterRight_SpringArm"));
	m_quarterRightSpringArm->SetupAttachment(RootComponent);

	m_quarterLeftSpringArm = CreateOptionalDefaultSubobject<USpringArmComponent>(TEXT("QuarterLeft_SpringArm"));
	m_quarterLeftSpringArm->SetupAttachment(RootComponent);

	m_pauseCameraSpringArm = CreateOptionalDefaultSubobject<USpringArmComponent>(TEXT("PauseCamera_SpringArm"));
	m_pauseCameraSpringArm->SetupAttachment(GetSkeletalMeshComponent());

	m_menuCameraSpringArm = CreateOptionalDefaultSubobject<USpringArmComponent>(TEXT("MenuCamera_SpringArm"));
	m_menuCameraSpringArm->SetupAttachment(RootComponent);
	
	m_quarterRightCameraActorComponent = CreateOptionalDefaultSubobject<UCameraChildActorComponent>(TEXT("QuarterRightCameraChildActor"));
	m_quarterRightCameraActorComponent->SetupAttachment(m_quarterRightSpringArm);

	m_quarterLeftCameraActorComponent = CreateOptionalDefaultSubobject<UCameraChildActorComponent>(TEXT("QarterLeftCameraChildActor"));
	m_quarterLeftCameraActorComponent->SetupAttachment(m_quarterLeftSpringArm);

	m_pauseCameraActorComponent = CreateOptionalDefaultSubobject<UCameraChildActorComponent>(TEXT("PauseCameraChildActor"));
	m_pauseCameraActorComponent->SetupAttachment(m_pauseCameraSpringArm);

	m_menuCameraActorComponent = CreateOptionalDefaultSubobject<UCameraChildActorComponent>(TEXT("MenuCameraChildActor"));
	m_menuCameraActorComponent->SetupAttachment(m_menuCameraSpringArm);
}

void APlayerEntity::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("PlayerEntity begin play"));

	//m_currentRail = RAIL_TYPES::RT_CENTER;
	m_rhythmHdl = ARhythmHandler::GetInstance();
	m_feedbackHdl = AFeedbackHandler::GetInstance();
	m_gameplaySpaceAnchor = AGameplaySpaceAnchor::GetInstance();

	// Setup Camera
	m_quarterRightCameraActorComponent->AttachToComponent(m_quarterRightSpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
	m_quarterLeftCameraActorComponent->AttachToComponent(m_quarterLeftSpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
	m_pauseCameraActorComponent->AttachToComponent(m_pauseCameraSpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
	m_menuCameraActorComponent->AttachToComponent(m_menuCameraSpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);

	FActorSpawnParameters parameters;
	parameters.Name = TEXT("ArmActor");
	parameters.Owner = this;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_armPlayer = GetWorld()->SpawnActor<AArmPlayer>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), parameters);
	m_armPlayer->m_owner = this;
	parameters.Name = TEXT("LegActor");
	m_legPlayer = GetWorld()->SpawnActor<ALegPlayer>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), parameters);
	m_legPlayer->m_owner = this;
}

void APlayerEntity::ResetSplineDistance()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetSplineDistance"));
	m_currentSplineDistance = 0.f;
}

void APlayerEntity::Init(ARail* newRail, UCurveFloat* newSpeedCurve)
{
	Super::Init(newRail);
	Reset();

	m_speedCurve = newSpeedCurve;

	UpdateMovementFromSpline(0.f);
	GetSkeletalMeshComponent()->SetRelativeLocation(FVector::ZeroVector);
	AActor* CameraActor = GetQuarterRightCameraChildActor();

	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ResetBeforeNewLevel"), ar, NULL, true);
}

void APlayerEntity::Reset()
{
	m_life = m_lifeMax;
	m_currentSplineDistance = 0;
	m_isDashing = false;
	m_speed = 0.f;
}

void APlayerEntity::ClearPlayerInput()
{
	UE_LOG(LogTemp, Warning, TEXT("ClearPlayerInput"));

	m_armPlayer->UnbindInput();
	m_legPlayer->UnbindInput();
}

void APlayerEntity::SetPlayerInput()
{
	UE_LOG(LogTemp, Warning, TEXT("SetPlayerInput"));


	m_armPlayer->BindInput();
	m_legPlayer->BindInput();
}

float APlayerEntity::TakeDamage(float amount, const FDamageEvent& dmgEvent, AController* ctrlr, AActor* actor)
{
	Super::TakeDamage(amount, dmgEvent, ctrlr, actor);
	
	m_life = m_life - amount;
	ComputeLifeState();

	//UE_LOG(LogTemp, Warning, TEXT("player loose %f health"), amount);

	//Feedback
	m_feedbackHdl->ShakeCamera(m_onDamageTakenCameraShake);
	
	//On Death
	if (m_life <= 0)
	{
		m_life = 0;

		//m_rhythmHdl->SetParameter("Fail", 1.f);
		ARhythmOfTheDemonGameStateBase* gameState = (ARhythmOfTheDemonGameStateBase*)GetWorld()->GetGameState();
		gameState->Lose();

		return 0.f;
	}
	OnTakeDamage_Event();
	return m_life;
}

void APlayerEntity::Regenerate(float amount)
{
	//UE_LOG(LogTemp, Warning, TEXT("player regen %f health"), amount);
	m_life = FMath::Clamp(m_life + amount, 0.f, m_lifeMax);
	ComputeLifeState();
	OnRegenerate.Broadcast();
}

void APlayerEntity::ComputeLifeState()
{
	float dangerLifeFactor = m_lifeMax / 2.5f;
	float goodLifeFactor = m_lifeMax / 1.42f;

	EPLAYER_LIFE_STATE res = EPLAYER_LIFE_STATE::None;
	int musicLayer = -1;

	if (m_life <= dangerLifeFactor)
	{
		res = EPLAYER_LIFE_STATE::Danger;
		musicLayer = 1;
	}
	else if (m_life <= goodLifeFactor)
	{
		res = EPLAYER_LIFE_STATE::Good;
		musicLayer = 2;
	}
	else
	{
		res = EPLAYER_LIFE_STATE::Great;
		musicLayer = 3;
	}

	float currentMusicLayer = m_rhythmHdl->GetParameter("Skill");

	if (m_playerLifeState == res && currentMusicLayer == musicLayer)
		return;

	m_playerLifeState = res;
	m_rhythmHdl->SetParameter("Skill", musicLayer);
}

bool APlayerEntity::IsDead()
{
	Super::IsDead();

	return false;
}

void APlayerEntity::EntityDie()
{
	Super::EntityDie();
}

void APlayerEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_rhythmHdl->IsPlaying())
		UpdateMovementFromSpline(DeltaTime);

	if (m_isDashing)
		ComputeDashMovement();

	if (!m_canDash)
	{
		m_dashTimer -= DeltaTime;
		if (m_dashTimer <= 0.f)
			m_canDash = true;
	}


	ComputeMovementLerp(DeltaTime);
}

void APlayerEntity::ComputeMovementLerp(float tick)
{
	m_yaw = FMath::Lerp(m_yaw, m_yawTargetValue, tick * m_yawLerpSpeed);
	m_pitch = FMath::Lerp(m_pitch, m_pitchTargetValue, tick * m_pitchLerpSpeed);
}

void APlayerEntity::UpdateMovementFromSpline(float deltaTime)
{
	//Update Speed
	float timePosition = m_rhythmHdl->GetTimelinePosition();

	m_speed = m_speedCurve->GetFloatValue(InSecond(timePosition));

	m_currentSplineDistance += m_speed * deltaTime;

	FTransform tsf = m_currentRail->GetSplineTransformAtDistance(m_currentSplineDistance);
	//FVector randOffset = m_randMvmtComponent->GetLocalSpaceOffset(GetActorForwardVector(), GetActorUpVector(), GetActorRightVector());
	
	SetActorLocation(tsf.GetLocation()/* + randOffset*/);
	SetActorRotation(tsf.GetRotation());

	m_gameplaySpaceAnchor->SetActorTransform(tsf);

	// reset SplineDistance if spline is looping and reach spline end
	if (m_currentRail->IsRailEnd(m_currentSplineDistance) && m_currentRail->IsRailLoop())
		ResetSplineDistance();
}

void APlayerEntity::SwitchControl()
{
	m_armPlayer->UnbindAllActions();
	m_legPlayer->UnbindAllActions();

	m_armPlayer->SwitchPlayerControl();
	m_legPlayer->SwitchPlayerControl();
}

void APlayerEntity::MoveRight(FVector newPos, float rightValue)
{
	if (m_isDashing)
		return;

	m_yawDeltaLerp = 0.f;
	m_yawTargetValue = rightValue;
	GetSkeletalMeshComponent()->SetRelativeLocation(newPos);
}

void APlayerEntity::MoveUp(FVector newPos, float upValue)
{
	if (m_isDashing)
		return;

	m_pitchDeltaLerp = 0.f;
	m_pitchTargetValue = upValue;
	GetSkeletalMeshComponent()->SetRelativeLocation(newPos);
}

EDASH_TYPE APlayerEntity::Dash(FVector DashPoint, FVector DashDirection)
{
	if (m_isDashing || !m_canDash)
		return EDASH_TYPE::None;

	float deltaTime = GetWorld()->DeltaTimeSeconds;
	n_meshDashStartPos = GetSkeletalMeshComponent()->RelativeLocation;
	m_dashFinalPoint = DashPoint;
	m_isDashing = true;
	m_canDash = false;
	m_dashTimer = m_dashCooldown;
	UFMODBlueprintStatics::PlayEvent2D(this, m_dashSound, true);

	//FVector normalizedDashDirection = DashPoint - GetSkeletalMeshComponent()->RelativeLocation;
	//normalizedDashDirection.Normalize();
	//Dash_Event(normalizedDashDirection);

	DashDirection.Normalize();
	Dash_Event(DashDirection);

	if (abs(m_pitch) > abs(m_yaw))
		return EDASH_TYPE::VerticalDash;
	else
		return EDASH_TYPE::HorizontalDash;
}

void APlayerEntity::ComputeDashMovement()
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	m_dashLerpDeltaTime += deltaTime * m_dashSpeed;

	FVector position;
	float delta = FMath::Clamp(m_dashLerpDeltaTime * m_dashSpeed, 0.f, 1.f);

	position = FMath::LerpStable(n_meshDashStartPos, m_dashFinalPoint, delta);

	GetSkeletalMeshComponent()->RelativeLocation = position;

	if (m_dashLerpDeltaTime >= 1.f)
	{
		m_dashLerpDeltaTime = 0.f;
		m_isDashing = false;
	}
}

void APlayerEntity::ShockWave()
{
	//UFMODBlueprintStatics::PlayEvent2D(this, m_shockwaveSound, true);
}

void APlayerEntity::RevertA()
{
	RevertA_Event();
}

void APlayerEntity::RevertB()
{
	RevertB_Event();
}

void APlayerEntity::RevertY()
{
	RevertY_Event();
}

void APlayerEntity::RevertX()
{
	RevertX_Event();
}