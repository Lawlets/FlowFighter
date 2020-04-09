// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation//AnimMontage.h"

#include "Enums/BrickState.h"

#include "BrickAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class RHYTHMOFTHEDEMON_API UBrickAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void PlayAnimFromState(EBRICK_STATE state);

protected:

	UFUNCTION(BlueprintCallable)
		void SetStateAnim(UAnimMontage* anim, EBRICK_STATE state);

private:

	UAnimMontage* m_idleStateAnim;
	UAnimMontage* m_prepStateAnim;
	UAnimMontage* m_hitStateAnim;
	UAnimMontage* m_passedStateAnim;
};
