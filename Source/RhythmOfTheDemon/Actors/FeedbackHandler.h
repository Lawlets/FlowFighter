// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Enums/SuccessType.h"
#include "Enums/PlayerType.h"

#include "Camera/CameraShake.h"

#include "FeedbackHandler.generated.h"

class ACameraHandler;

UCLASS()
class RHYTHMOFTHEDEMON_API AFeedbackHandler : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFeedbackHandler();
	static AFeedbackHandler* GetInstance() { return m_instance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	static AFeedbackHandler* m_instance;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void ShowTextFeedback(ESUCCESS_TYPE success, EPLAYER_TYPE target);

	UFUNCTION(BlueprintImplementableEvent)
		void ShakeController(EPLAYER_TYPE target, UForceFeedbackEffect* shakeEffect);


	void ShakeController(EPLAYER_TYPE target);
	void ShakeCamera(TSubclassOf<UCameraShake> camShake);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UForceFeedbackEffect* m_defaultShakeEffect;

	ACameraHandler* m_camHdl;
};
