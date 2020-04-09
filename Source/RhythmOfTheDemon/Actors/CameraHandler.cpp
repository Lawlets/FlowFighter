// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraHandler.h"

#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Math/Color.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ACameraHandler* ACameraHandler::m_instance;

// Sets default values
ACameraHandler::ACameraHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_cameraActorMap = TMap<ECAMERA_VIEW, AActor*>();

	m_instance = this;
}

// Called when the game starts or when spawned
void ACameraHandler::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	m_playerCameraManager = playerController->PlayerCameraManager;

	m_playerCameraManager->SetViewTarget(GetCameraActorFromMap(ECAMERA_VIEW::MENU_IDLE));
	m_currentCamera = GetCameraActorFromMap(ECAMERA_VIEW::MENU_IDLE);
	m_isFadingCamera = false;
	//FadeOutCurrentCamera();
	//BlendTo(ECAMERA_VIEW::MENU_IDLE);
}

// Called every frame
void ACameraHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* ACameraHandler::AddCameraActorToMap(ECAMERA_VIEW key, AActor * cameraActor)
{
	if (m_cameraActorMap.Contains(key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Key is already contains in CameraActorMap"));
		//UE_LOG(LogTemp, Warning, TEXT("key [%s] is already contains in CameraActorMap"), *GETENUMSTRING("ECAMERA_VIEW", key));
		return nullptr;
	}

	m_cameraActorMap.Add(key, cameraActor);
	return cameraActor;
}

AActor * ACameraHandler::ReplaceCameraActorInMap(ECAMERA_VIEW key, AActor * cameraActor)
{
	if (!m_cameraActorMap.Contains(key))
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraActorMap doesn't contain key"));
		return nullptr;
	}

	m_cameraActorMap[key] = cameraActor;
	return cameraActor;
}

void ACameraHandler::Shake(TSubclassOf<UCameraShake> shakeParam)
{
	m_playerCameraManager->PlayCameraShake(shakeParam);
}

ECAMERA_VIEW ACameraHandler::GetCameraKeyFromActor(AActor* cameraActor)
{
	const ECAMERA_VIEW* key = m_cameraActorMap.FindKey(cameraActor);
	return *key;
}

void ACameraHandler::BlendTo(ECAMERA_VIEW key, bool blendWithoutTransition)
{
	if (m_currentCamera != nullptr)
	{
		ECAMERA_VIEW key = GetCameraKeyFromActor(m_currentCamera);;
		m_lastCameraKey = key;
	}

	AActor* cameraActor = m_cameraActorMap[key];
	if (cameraActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraActor is null"));
		//UE_LOG(LogTemp, Warning, TEXT("Camera actor at key [%s] is null"), *GETENUMSTRING("ECAMERA_VIEW", key));
		return;
	}

	if (blendWithoutTransition)
		m_playerCameraManager->SetViewTarget(cameraActor);
	else
	{
		m_playerCameraManager->SetViewTarget(cameraActor, m_transitionParams);
		GetWorldTimerManager().SetTimer(m_blendTimerHandle, this, &ACameraHandler::BlendCameraFinish, m_transitionParams.BlendTime, false);
		m_isBlending = true;
	}

	m_currentCamera = cameraActor;
}

void ACameraHandler::BlendCameraFinish()
{
	GetWorldTimerManager().ClearTimer(m_blendTimerHandle);

	m_isBlending = false;
}

void ACameraHandler::FadeOutCurrentCamera()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);

	m_playerCameraManager->StartCameraFade(0.f, 1.f, m_fadeDuration, FLinearColor::Black, false, true);
	GetWorldTimerManager().SetTimer(timerHandle, this, &ACameraHandler::BroadcastFadeOutEvent, m_fadeDuration, false);
}

void ACameraHandler::FadeInCurrentCamera()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);

	m_playerCameraManager->StartCameraFade(2.f, 0.f, m_fadeDuration, FLinearColor::Black, false, true);
	GetWorldTimerManager().SetTimer(timerHandle, this, &ACameraHandler::BroadcastFadeInEvent, m_fadeDuration, false);
}

void ACameraHandler::BroadcastFadeOutEvent()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	FadeOutEnd.Broadcast();
}

void ACameraHandler::BroadcastFadeInEvent()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	FadeInEnd.Broadcast();
}