// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StreakComponnent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RHYTHMOFTHEDEMON_API UStreakComponnent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStreakComponnent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool HasStreak() const { return m_currentStreak > 0; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStreakModifier_Event);

	UFUNCTION(BlueprintCallable)
		int GetStreak() const { return m_currentStreak; }
	UFUNCTION(BlueprintCallable)
		int GetBestStreak() const { return m_bestStreak; }
	
	int IsInGreatStreak() const { return m_currentStreak >= 10; }

	UPROPERTY(BlueprintAssignable)
		FStreakModifier_Event OnAddStreak;

	UPROPERTY(BlueprintAssignable)
		FStreakModifier_Event OnResetStreak;

	void CleanAllStreak();

	void ResetStreak();
	void AddStreak();

private:
	void UpdateBestStreak();

private:
	int m_currentStreak;
	int m_bestStreak;
	
};
