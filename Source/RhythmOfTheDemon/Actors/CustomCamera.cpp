// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomCamera.h"

#include "Kismet/KismetMathLibrary.h"

ACustomCamera::ACustomCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACustomCamera::BeginPlay()
{
	Super::BeginPlay();
}


void ACustomCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_isFollowingTarget)
		FollowTarget();
}

void ACustomCamera::FollowTarget()
{
	if (m_target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera can't follow a nullptr target"));
		return;
	}

	//FVector targetLocation = m_target->GetActorLocation();
	//FRotator targetRotator = m_target->GetActorRotation();
	//
	//FVector currentLocation = GetActorLocation();
	//FRotator currentRotator = GetActorRotation();
	//
	//FVector actor_target_direction = targetLocation - currentLocation;
	//actor_target_direction.Normalize();
	//FVector scaledDirection = actor_target_direction * m_distanceToTarget;
	//
	//FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	//
	//FVector newLocation = UKismetMathLibrary::VLerp(currentLocation, scaledDirection + currentLocation, 0.01f);
	//
	//SetActorLocationAndRotation(newLocation, lookAtRotator);
}