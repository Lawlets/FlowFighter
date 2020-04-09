// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FMODEvent.h"

#include "Actors/Components/DissolveComponent.h"
#include "Actors/Rail.h"
#include "AbstractEntityActor.generated.h"

class USkeletalMeshComponent;
class URandomMovementComponent;


UCLASS()
class RHYTHMOFTHEDEMON_API AAbstractEntityActor : public AActor
{
	GENERATED_BODY()

public:
	AAbstractEntityActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Init(ARail* newRail);

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent*		GetSkeletalMeshComponent() const { return m_skeletalMeshComponent; }
	USceneComponent*			GetHitPoint() const { return m_hitPoint; }
	USceneComponent*			GetCastPoint() const { return m_castPoint; }
	ARail*						GetCurrentRail() const { return m_currentRail; }
	virtual float				GetCurrentSplineDistance() { return 0; }

	void						SetLife(float value) { m_life = value; }
	void						SetSkeletalMeshComponent(USkeletalMeshComponent* value) { m_skeletalMeshComponent = value; }

	UFUNCTION(BlueprintCallable)
		float						GetLife() const { return m_life; }

	UFUNCTION(BlueprintCallable)
		virtual float TakeDamage(float amount, const FDamageEvent& dmgEvent, AController* ctrlr, AActor* actor);

	virtual void SetVisible(bool visible, float duration);

protected:

	virtual void BeginPlay() override;
	virtual bool IsDead();

	UFUNCTION(BlueprintCallable)
		virtual void EntityDie();

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void OnMatInstanciated();

	UFUNCTION(blueprintImplementableEvent, Category = "Events")
		void OnTakeDamage_Event();

private:
	void Hide();

public:
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* m_skeletalMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USceneComponent* m_root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USceneComponent* m_hitPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USceneComponent* m_castPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		URandomMovementComponent* m_randMvmtComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UDissolveComponent* m_dissolveComponent;

	UPROPERTY(EditAnywhere, Category = "Sound")
		UFMODEvent* m_damageTakenSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_lifeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_life;

	UPROPERTY(BlueprintReadOnly)
		UMaterialInstanceDynamic* m_dynamicMaterial;

	ARail* m_currentRail;
};
