// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossPawn.h"
#include "Actors/PlayerEntity.h"
#include "Actors/RailHandler.h"
#include "Actors/Rail.h"
#include "Actors/Components/RandomMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
//#include "Engine/Classes/Debug/DebugDrawService.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

// Sets default values
AMiniBossPawn::AMiniBossPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_damageTakenAnimComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("DamageTakenAnimation"));
	m_damageTakenAnimComponent->SetupAttachment(m_skeletalMeshComponent);
}

// Called when the game starts or when spawned
void AMiniBossPawn::BeginPlay()
{
	Super::BeginPlay();

	m_damageTakenAnimComponent->Stop();
	m_damageTakenAnimComponent->SetLooping(false);
}

void AMiniBossPawn::Init(ARail* newRail)
{
	Super::Init(newRail);

	m_distanceFromPlayer = m_initialDistFromPlayer;
}

// Called every frame
void AMiniBossPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Position itself according to player
	if (!m_player)
		return;

	AdjustDistFromPlayer(DeltaTime);

	float playerPosition = m_player->GetCurrentSplineDistance();
	float distanceOnSpline = ManageSplineLooping(playerPosition);

	FTransform tsf = m_currentRail->GetSplineTransformAtDistance(distanceOnSpline);

	SetActorLocation(tsf.GetLocation());
	SetActorRotation(tsf.GetRotation());

	LookAtTarget(m_player);
}

float AMiniBossPawn::GetCurrentSplineDistance()
{
	return m_player->GetCurrentSplineDistance() + m_distanceFromPlayer;
}

float AMiniBossPawn::TakeDamage(float amount, const FDamageEvent& dmgEvent, AController* ctrlr, AActor* actor)
{
	Super::TakeDamage(amount, dmgEvent, ctrlr, actor);

	//UE_LOG(ApplicationInitLog, Log, TEXT("* IA take %f damage !"), amount);

	//Anim Feedback
	if (m_damageTakenAnimArray.Num() > 0)
	{
		int randAnim = FMath::RandRange(0, m_damageTakenAnimArray.Num() - 1);
		m_damageTakenAnimComponent->SetFlipbook(m_damageTakenAnimArray[randAnim]);
	}
	m_damageTakenAnimComponent->PlayFromStart();

	m_life -= amount;

	if (!m_isOverkilled)
	{
		if (m_life <= 0.f)
		{
			m_isOverkilled = true;
			OnOverkill();
		}
	}

	return amount;
}

void AMiniBossPawn::AdjustDistFromPlayer(float DeltaTime)
{
	FVector playerLoc = m_player->GetActorLocation();
	FVector distVec = GetActorLocation() - playerLoc;

	distVec = FVector::VectorPlaneProject(distVec, m_player->GetActorRightVector());
	distVec = FVector::VectorPlaneProject(distVec, m_player->GetActorUpVector());

	float dist = distVec.Size();
	distVec.Normalize();

	FVector perpendicularVec = distVec.CrossProduct(m_player->GetActorRightVector(), distVec);
	if (FVector::DotProduct(perpendicularVec, m_player->GetActorUpVector()) > 0.f)
		dist *= -1.f;
	
	//DrawDebugLine(GetWorld(), playerLoc, playerLoc + (distVec * dist), FColor(255, 0, 0), false, -1, 0, 7.f);
	//UE_LOG(LogTemp, Warning, TEXT("current dist between entity = %f"), dist);

	if (dist + m_minDistGapFromPlayer > m_absoluteDistFromPlayer)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Go Closer"));
		m_distanceFromPlayer -= DeltaTime * m_distUpdateSpeed;
	}
	else if (dist - m_minDistGapFromPlayer < m_absoluteDistFromPlayer)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Go Further"));
		m_distanceFromPlayer += DeltaTime * m_distUpdateSpeed;
	}
}

float AMiniBossPawn::ManageSplineLooping(float playerPosition)
{
	float result = playerPosition + m_distanceFromPlayer;

	if (m_currentRail->IsRailLoop() && m_currentRail->IsRailEnd(playerPosition + m_distanceFromPlayer))
		result = (playerPosition + m_distanceFromPlayer) - m_currentRail->GetSplineLength();

	return result;
}

void AMiniBossPawn::LookAtTarget(AActor* target)
{
	FVector bossForward = -GetActorForwardVector();
	FVector bossUp = GetActorUpVector();

	// Compute LookAt Vector
	FVector targetLoc = target->GetActorLocation();
	FVector myMeshLoc = m_skeletalMeshComponent->GetComponentLocation();
	FVector lookAtDir = (targetLoc - myMeshLoc);

	lookAtDir = FVector::VectorPlaneProject(lookAtDir, bossUp);
	lookAtDir.Normalize();

	// Compute Angle Value
	float LookAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(bossForward, lookAtDir)));

	// Check Angle Sign
	FVector perpendicularVec = bossForward.CrossProduct(bossForward, lookAtDir);
	if (FVector::DotProduct(perpendicularVec, bossUp) < 0.f)
		LookAtAngle *= -1.f;

	// ** DEBUG **
	//UE_LOG(LogTemp, Warning, TEXT("Angle between basic & look at = %f"), LookAtAngle);
	//float lineDist = 60.f;
	//DrawDebugLine(GetWorld(), myMeshLoc, myMeshLoc + (bossForward * lineDist),FColor(0, 0, 255), false, -1, 0, 7.f);
	//DrawDebugLine(GetWorld(), myMeshLoc, myMeshLoc + (lookAtDir * lineDist), FColor(0, 255, 0), false, -1, 0, 7.f);

	//Rotate Boss
	FRotator bossRotator = FRotator(0.f, 90.f + LookAtAngle, 0.f);
	m_skeletalMeshComponent->SetRelativeRotation(bossRotator);
}
