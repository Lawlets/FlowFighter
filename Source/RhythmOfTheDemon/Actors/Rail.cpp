// Fill out your copyright notice in the Description page of Project Settings.

#include "Rail.h"

#include "Components/SplineComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

#include "Engine/World.h"
#include "RhythmOfTheDemonGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/PlayerEntity.h"
#include "Curves/RichCurve.h"
#include "Sound/RhythmHandler.h"


ARail::ARail()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponentRoot");

	m_spline = CreateDefaultSubobject<USplineComponent>(TEXT("RailSpline"));
	m_spline->SetupAttachment(RootComponent);
}

void ARail::BeginPlay()
{
	Super::BeginPlay();
	m_endSplineLocation = m_spline->GetLocationAtSplinePoint(m_spline->GetSplineLength(), ESplineCoordinateSpace::World);
}

void ARail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ARail::IsRailEnd(float distance)
{
	if (distance > m_spline->GetSplineLength())
		return true;
	return false;
}

bool ARail::IsRailLoop()
{
	return m_spline->IsClosedLoop();
}

float ARail::GetSplineLength()
{
	return m_spline->GetSplineLength();
}

FTransform ARail::GetSplineTransformAtTimelinePosition(int timelinePosition)
{
	return m_spline->GetTransformAtTime(timelinePosition, ESplineCoordinateSpace::World);
}

FTransform ARail::GetSplineTransformAtDistance(float distance)
{
	FTransform splineTransform = m_spline->GetTransformAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	return splineTransform;
}
