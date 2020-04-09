// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractBrick.h"
#include "FMODEvent.h"

#include "Engine.h"
#include "AbstractProjectile.generated.h"

class AAbstractEntityActor;
class ARhythmHandler;

class UPaperFlipbookComponent;
class UPaperFlipbook;
class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class RHYTHMOFTHEDEMON_API AAbstractProjectile : public AAbstractBrick
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbstractProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//void Destroy();

public:
	virtual void Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Update(int timelinePosition) override;

	virtual EINTERACTION_RESULT Interact(float timelinePos, bool isFromArmPlayer) override;

	virtual void SwitchState(EBRICK_STATE newState);

protected:
	void Reflect();
	FVector ComputePosition(FVector begin, FVector end, float progression);

	void OnSwitchToHitState();
	void OnReflectFeedback();
	void OnMissedFeedback();

	void SetSuccessType(ESUCCESS_TYPE success) override;

	virtual void Show() override;
	virtual void Hide() override;

	bool m_isReflected = false;

	//Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* m_mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* m_afterReflectionParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPaperFlipbookComponent* m_reflectFlipbookComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* m_onMissParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* m_onSwitchHitParticle;
	
	//Parameter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UFMODEvent* m_reflectSound;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		FVector m_curveDirection;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float m_curveIntensity = 50.f;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		UCurveFloat* m_pathCurve;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		UCurveFloat* m_reflectedPathCurve;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float m_reflectTravelDuration = 1000.f;

	/*UPROPERTY(EditAnywhere, Category = "Graphics")
		UPaperFlipbook* m_reflectAnim;*/

	UPROPERTY(EditAnywhere, Category = "Graphics")
		TArray<UPaperFlipbook*> m_reflectAnims;

	int m_lastTimePos = 0;
	FVector m_lockOffset;
	UCurveFloat* m_currPath;
};
