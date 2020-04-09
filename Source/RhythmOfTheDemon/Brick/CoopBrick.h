// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractBrick.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"

#include "FMODEvent.h"
#include "CoopBrick.generated.h"

UCLASS()
class RHYTHMOFTHEDEMON_API ACoopBrick : public AAbstractBrick
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoopBrick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Update(int timelinePosition) override;

	virtual EINTERACTION_RESULT Interact(float timelinePos, bool isFromArmPlayer) override;

	virtual void SwitchState(EBRICK_STATE newState);

protected:
	void Blink(float blinkduration);

	void OnDeviateBoss();
	void SetSuccessType(ESUCCESS_TYPE success) override;

	virtual void Show() override;
	virtual void Hide() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* m_mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* m_shieldParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graphics")
		TArray<UParticleSystem*> m_shieldParticleArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UFMODEvent* m_shockwaveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UFMODEvent* m_step1Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UFMODEvent* m_step2Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UFMODEvent* m_step3Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_dissolveDuration = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_idlePositionRatio = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_prepPositionRatio = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_hitPositionRatio = 0.05f;

	bool m_armPlayerActivated = false;
	bool m_legPlayerActivated = false;
};
