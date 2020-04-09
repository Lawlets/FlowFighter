// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyPawn.h"
#include "Brick/BricksHandler.h"
#include "Brick/AbstractBrick.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	m_isOverkilled = false;
}

void AEnemyPawn::Init(ARail* newRail)
{
	Super::Init(newRail);

	//Init LifePoint
	ABricksHandler* brickHdl = ABricksHandler::GetInstance();
	float projCountValue = brickHdl->GetLevelProjectileCount() * m_projectileMaxDmg;
	float coopCountValue = brickHdl->GetLevelCoopCount() * m_coopMaxDmg;

	float absoluteLife = projCountValue + coopCountValue;
	
	m_lifeMax = m_life = absoluteLife * projectileRatioToMaxLife;
	m_maxOverkillValue = absoluteLife - m_lifeMax;
}

void AEnemyPawn::CastProjectiles(AAbstractBrick* brick)
{
	CastProjEvent(brick);
}

