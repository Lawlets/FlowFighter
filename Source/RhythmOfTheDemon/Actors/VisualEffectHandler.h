// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "Engine.h"

#include "VisualEffectHandler.generated.h"

class ARhythmHandler;

UCLASS()
class RHYTHMOFTHEDEMON_API AVisualEffectHandler : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVisualEffectHandler();
	static AVisualEffectHandler* GetInstance() { return m_instance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	static AVisualEffectHandler* m_instance;

public:



private:
	ARhythmHandler* m_rhythmHdl;
	AWorldSettings* m_worldSettings;

	float m_currentTimeDilatation;
	float m_slowTimeDuration;
	bool m_isTimeGrowing;

};
