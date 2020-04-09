// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualEffectHandler.h"


#include "Sound/RhythmHandler.h"

AVisualEffectHandler* AVisualEffectHandler::m_instance;

// Sets default values
AVisualEffectHandler::AVisualEffectHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	m_instance = this;
}

// Called when the game starts or when spawned
void AVisualEffectHandler::BeginPlay()
{
	Super::BeginPlay();

	m_rhythmHdl = ARhythmHandler::GetInstance();
}

