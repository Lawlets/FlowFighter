// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplaySpaceAnchor.h"

AGameplaySpaceAnchor* AGameplaySpaceAnchor::m_instance;

// Sets default values
AGameplaySpaceAnchor::AGameplaySpaceAnchor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	m_instance = this;
}

// Called when the game starts or when spawned
void AGameplaySpaceAnchor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AGameplaySpaceAnchor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

