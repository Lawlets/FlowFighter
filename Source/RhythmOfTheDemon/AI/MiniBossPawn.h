// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyPawn.h"
#include "Actors/AbstractEntityActor.h"
#include "MiniBossPawn.generated.h"

class ARail;
class UPaperFlipbook;
class UPaperFlipbookComponent;

/**
 * 
 */
UCLASS()
class RHYTHMOFTHEDEMON_API AMiniBossPawn : public AEnemyPawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AMiniBossPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(ARail* newRail) override;

	virtual float GetCurrentSplineDistance() override;

	virtual float TakeDamage(float amount, const FDamageEvent& dmgEvent, AController* ctrlr, AActor* actor) override;


protected:

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void OnOverkill();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance From Player")
		float m_initialDistFromPlayer = 125.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance From Player")
		float m_absoluteDistFromPlayer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance From Player")
		float m_distUpdateSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance From Player")
		float m_minDistGapFromPlayer = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPaperFlipbookComponent* m_damageTakenAnimComponent;

	UPROPERTY(EditAnywhere)
		TArray<UPaperFlipbook*> m_damageTakenAnimArray; //= TArray<UPaperFlipbook>();

	void AdjustDistFromPlayer(float DeltaTime);
	float ManageSplineLooping(float playerPosition);

	void LookAtTarget(AActor* target);

	float m_distanceFromPlayer;
};
