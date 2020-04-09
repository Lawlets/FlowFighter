// Fill out your copyright notice in the Description page of Project Settings.

#include "FeedbackHandler.h"
#include "Actors/CameraHandler.h"

AFeedbackHandler* AFeedbackHandler::m_instance;

// Sets default values
AFeedbackHandler::AFeedbackHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_instance = this;
}

// Called when the game starts or when spawned
void AFeedbackHandler::BeginPlay()
{
	Super::BeginPlay();

	m_camHdl = ACameraHandler::GetInstance();
}

void AFeedbackHandler::ShakeCamera(TSubclassOf<UCameraShake> camShake)
{
	m_camHdl->Shake(camShake);
}

void AFeedbackHandler::ShakeController(EPLAYER_TYPE target)
{
	ShakeController(target, m_defaultShakeEffect);
}
