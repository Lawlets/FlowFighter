// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Actors/AbstractEntityActor.h"
#include "EnemyPawn.generated.h"

class APlayerEntity;
class AAbstractBrick;

UCLASS()
class RHYTHMOFTHEDEMON_API AEnemyPawn : public AAbstractEntityActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Init(ARail* newRail) override;

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void CastProjEvent(AAbstractBrick* brick);

	virtual void CastProjectiles(AAbstractBrick* brick);

	APlayerEntity* m_player;
	
protected: 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_projectileMaxDmg = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_coopMaxDmg = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float projectileRatioToMaxLife = 0.5f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	float projectileRatioToMaxOverkill = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_maxOverkillValue = 1000.f;

	bool m_isOverkilled = false;
};
