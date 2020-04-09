// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomMovementComponent.h"


// Sets default values for this component's properties
URandomMovementComponent::URandomMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URandomMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void URandomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_positionTimer += DeltaTime / m_timeBetweenPosition;

	if (m_positionTimer >= 1)
		ComputeNextOffset();

	m_currentOffset = FMath::Lerp(m_lastOffset, m_nextOffset, m_positionTimer);
}

FVector URandomMovementComponent::GetLocalSpaceOffset(FVector forward, FVector up, FVector right)
{
	return right * m_currentOffset.X + up * m_currentOffset.Y + forward * m_currentOffset.Z;
}

void URandomMovementComponent::ComputeNextOffset()
{
	m_positionTimer = 0.f;
	m_lastOffset = m_nextOffset;

	m_nextOffset = FVector(
		FMath::RandRange(-m_distMax.X, m_distMax.X),
		FMath::RandRange(-m_distMax.Y, m_distMax.Y),
		FMath::RandRange(-m_distMax.Z, m_distMax.Z));

}
