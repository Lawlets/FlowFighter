// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CustomCamera.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMOFTHEDEMON_API ACustomCamera : public ACameraActor
{
	GENERATED_BODY()
	
	
public:
	ACustomCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION(BlueprintCallable)
		void FollowTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
		bool m_isFollowingTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
		AActor* m_target = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_distanceToTarget = 0.f;
};
