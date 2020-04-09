// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineLevel.h"
#include "Actors/RailHandler.h"

// Sets default values
ASplineLevel::ASplineLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = root;

	m_spline = CreateDefaultSubobject<USplineComponent>("LevelSpline");
	m_spline->SetupAttachment(root);
}

// Called when the game starts or when spawned
void ASplineLevel::BeginPlay()
{
	Super::BeginPlay();
	
	//m_railHandler = ARailHandler::GetInstance();
	//
	//if (m_railHandler != nullptr)
	//	m_railHandler->SetupSplineCurve(m_spline->SplineCurves);
	//else
	//	UE_LOG(LogTemp, Log, TEXT("RailHandler is null in SplineLevel"));
}

// Called every frame
void ASplineLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FSplineCurves ASplineLevel::GetCurve()
{
	return FSplineCurves();
}

