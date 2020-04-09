// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/CameraType.h"
#include "Camera/PlayerCameraManager.h"
#include "HAL/Event.h"
#include "CameraHandler.generated.h"

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

DECLARE_EVENT(ACameraHandler, OnFadeOutEnd);
DECLARE_EVENT(ACameraHandler, OnFadeInEnd);
DECLARE_DELEGATE(DelegateFunction);

UCLASS()
class RHYTHMOFTHEDEMON_API ACameraHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraHandler();
	static ACameraHandler* GetInstance() { return m_instance; }

protected:
	virtual void BeginPlay() override;
	static ACameraHandler* m_instance;

public:	
	virtual void Tick(float DeltaTime) override;
	
	TMap<ECAMERA_VIEW, AActor*> GetCameraActorMap() const { return m_cameraActorMap; }
	ECAMERA_VIEW GetCameraKeyFromActor(AActor* cameraActor);
	AActor* GetCameraActorFromMap(ECAMERA_VIEW key) { return m_cameraActorMap[key]; }
	AActor* AddCameraActorToMap(ECAMERA_VIEW key, AActor* cameraActor);
	AActor* ReplaceCameraActorInMap(ECAMERA_VIEW key, AActor* cameraActor);
	AActor* GetCurrentCamera() { return m_currentCamera; }

	DelegateFunction FadeDelegate;

	void Shake(TSubclassOf<UCameraShake> shakeParam);
	UFUNCTION(BlueprintCallable)
		void BlendTo(ECAMERA_VIEW key, bool blendWithoutTransition = false);
	UFUNCTION(BlueprintCallable)
		ECAMERA_VIEW GetLastCameraKey() const { return m_lastCameraKey; }

	void FadeOutCurrentCamera();
	void FadeInCurrentCamera();
	void BlendCameraFinish();

	OnFadeOutEnd FadeOutEnd;
	OnFadeInEnd FadeInEnd;

	bool GetFadingState() const { return m_isFadingCamera;}

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float m_fadeDuration = 1.f;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
		TMap<ECAMERA_VIEW, AActor*> m_cameraActorMap;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FViewTargetTransitionParams m_transitionParams;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_isBlending;

	ECAMERA_VIEW m_lastCameraKey;

	APlayerCameraManager* m_playerCameraManager;
	AActor* m_currentCamera = nullptr;

	FTimerHandle timerHandle;
	FTimerHandle m_blendTimerHandle;

	bool m_isFadingCamera = false;

	void BroadcastFadeOutEvent();
	void BroadcastFadeInEvent();
};
