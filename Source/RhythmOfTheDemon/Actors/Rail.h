// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"
#include "Rail.generated.h"

class USplineComponent;
class ARhythmHandler;

UCLASS()
class RHYTHMOFTHEDEMON_API ARail : public AActor
{
	GENERATED_BODY()

public:
	ARail();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FTransform GetSplineTransformAtTimelinePosition(int timelinePosition);

	UFUNCTION(BlueprintCallable)
		FTransform GetSplineTransformAtDistance(float distance);

	USplineComponent*	GetSpline() const { return m_spline; }

	bool IsRailEnd(float distance);
	bool IsRailLoop();
	float GetSplineLength();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USplineComponent* m_spline;

	FVector m_endSplineLocation;
};
