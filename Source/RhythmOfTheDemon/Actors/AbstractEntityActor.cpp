// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractEntityActor.h"
#include "Components/SkeletalMeshComponent.h"

#include "FMODBlueprintStatics.h"

#include "Actors/Components/RandomMovementComponent.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

AAbstractEntityActor::AAbstractEntityActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;

	m_root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = m_root;

	m_skeletalMeshComponent = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("EntityMesh"));
	m_skeletalMeshComponent->SetupAttachment(RootComponent, TEXT("SkeletalMesh"));

	m_hitPoint = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("hitPoint"));
	m_hitPoint->SetupAttachment(m_skeletalMeshComponent);

	m_castPoint = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("castPoint"));
	m_castPoint->SetupAttachment(m_skeletalMeshComponent);

	m_randMvmtComponent = this->CreateOptionalDefaultSubobject<URandomMovementComponent>(TEXT("RandomMovementGenerator"));
	this->AddOwnedComponent(m_randMvmtComponent);

	this->RemoveInstanceComponent(m_dissolveComponent);
	m_dissolveComponent = CreateDefaultSubobject<UDissolveComponent>(FName("DissolveComponent"));
	this->AddInstanceComponent(m_dissolveComponent);
}

void AAbstractEntityActor::BeginPlay()
{
	Super::BeginPlay();

	m_dynamicMaterial = UMaterialInstanceDynamic::Create(m_skeletalMeshComponent->GetMaterial(0), this, FName("Mat Instance"));
	m_skeletalMeshComponent->SetMaterial(0, m_dynamicMaterial);

	m_dissolveComponent->Init(m_dynamicMaterial, &GetWorldTimerManager());
	m_dissolveComponent->Show();

	m_dissolveComponent->OnDissolveComplete().AddUObject(this, &AAbstractEntityActor::Hide);

	OnMatInstanciated();
}

void AAbstractEntityActor::Init(ARail* newRail)
{
	m_currentRail = newRail;
}

void AAbstractEntityActor::SetVisible(bool visible, float duration)
{
	if (visible)
	{
		SetActorHiddenInGame(false);
		m_dissolveComponent->Ressolve(duration);
	}
	else
	{
		m_dissolveComponent->Dissolve(duration);
	}
}

float AAbstractEntityActor::TakeDamage(float amount, const FDamageEvent& dmgEvent, AController* ctrlr, AActor* actor)
{
	//Generic Feedback
	UFMODBlueprintStatics::PlayEvent2D(this, m_damageTakenSFX, true);

	OnTakeDamage_Event();
	return amount;
}

bool AAbstractEntityActor::IsDead()
{
	return m_life <= 0.f;
}

void AAbstractEntityActor::EntityDie()
{
}

void AAbstractEntityActor::Hide()
{
	SetActorHiddenInGame(true);
}

void AAbstractEntityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

