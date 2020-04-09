// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameplaySpaceAnchor.generated.h"

UCLASS()
class RHYTHMOFTHEDEMON_API AGameplaySpaceAnchor : public AActor
{
	GENERATED_BODY()

private:
	static AGameplaySpaceAnchor* m_instance;

public:
	// Sets default values for this actor's properties
	AGameplaySpaceAnchor();
	static AGameplaySpaceAnchor* GetInstance() { return m_instance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
};
