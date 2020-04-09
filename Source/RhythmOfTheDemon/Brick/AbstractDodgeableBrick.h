// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "Enums/InteractionResult.h"
#include "FMODEvent.h"

#include "AbstractBrick.h"
#include "AbstractDodgeableBrick.generated.h"

class ARhythmHandler;
class ARailHandler;

class AAbstractEntityActor;
class ARail;

class UPaperFlipbookComponent;
//class UParticleSystem;
class UCurveFloat;

/**
 *
 */
UCLASS()
class RHYTHMOFTHEDEMON_API AAbstractDodgeableBrick : public AAbstractBrick
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbstractDodgeableBrick(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	/*~AAbstractDodgeableBrick();*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual EINTERACTION_RESULT Interact(float timelinePos, bool isFromArmPlayer);

	virtual void SwitchState(EBRICK_STATE newState);

public:
	// Called every frame
	virtual void Init(int travelTime, AAbstractEntityActor* caster, AAbstractEntityActor* target) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Update(int timelinePosition) override;

	virtual EINTERACTION_RESULT Interact(float timelinePos, bool isFromArmPlayer) override;

protected:

	void ComputeHandLocation(int timelinePosition, FVector direction, USkeletalMeshComponent* handMesh, FName offsetSocketName);
	void LockPosition();

	void OnClapFeedback();
	void SetSuccessType(ESUCCESS_TYPE success) override;

	virtual void Show() override;
	virtual void Hide() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* m_leftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* m_rightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPaperFlipbookComponent* m_clapFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* m_clapParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* m_validDashFXComp;

	UPROPERTY(EditAnywhere, Category = "Sound")
		UFMODEvent* m_clapSFX;

	UPROPERTY(EditAnywhere)
		FName m_leftHandSocket = "Left_Hand_Socket";

	UPROPERTY(EditAnywhere)
		FName m_rightHandSocket = "Right_Hand_Socket";

	UPROPERTY(EditAnywhere)
		float m_distOffsetFromPlayer = 20.f;

	UPROPERTY(EditAnywhere)
		bool m_isMvmtHorizontal = true;

	UPROPERTY(EditAnywhere)
		float m_curveIntensity = 20.f;

	UPROPERTY(EditAnywhere, Category = "Graphics")
		float m_waitBeforeClap = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Graphics")
		TArray<UParticleSystem*> m_dashFXs;

	UBrickAnimInstance* m_secondHandAnimInstance;

	UCurveFloat* m_pathCurve;

	ARail* m_currRail;
	FVector m_lockPositionOffset;
};
