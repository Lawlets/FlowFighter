// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RandomMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RHYTHMOFTHEDEMON_API URandomMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URandomMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetLocalSpaceOffset(FVector forward, FVector up, FVector right);

//	FVector GetCurrentOffset() { return m_currentOffset; }
protected:

	void ComputeNextOffset();

	UPROPERTY(EditAnywhere)
		float m_timeBetweenPosition = 1.f;

	UPROPERTY(EditAnywhere)
		FVector m_distMax;


private:

	FVector m_currentOffset;

	FVector m_lastOffset;
	FVector m_nextOffset;

	float m_positionTimer = 0.f;
	
};
