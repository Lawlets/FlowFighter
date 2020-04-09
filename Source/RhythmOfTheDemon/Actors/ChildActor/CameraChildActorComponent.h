// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "CameraChildActorComponent.generated.h"


class UCameraComponent;

/**
 * 
 */
UCLASS()
class RHYTHMOFTHEDEMON_API UCameraChildActorComponent : public UChildActorComponent
{
	GENERATED_BODY()
	
public:
	UCameraChildActorComponent();

protected:
	virtual void BeginPlay() override;

private:
};
