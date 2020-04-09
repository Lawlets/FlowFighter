// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

#include "DissolveComponent.generated.h"

DECLARE_EVENT(UDissolveComponent, RessolveComplete);
DECLARE_EVENT(UDissolveComponent, DissolveComplete);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RHYTHMOFTHEDEMON_API UDissolveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDissolveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init(UMaterialInstanceDynamic* dissolveMaterial, FTimerManager* timer);

	void Ressolve(float duration);
	void Ressolve(float duration, float delay);

	void Dissolve(float duration);
	void Dissolve(float duration, float delay);

	RessolveComplete& OnRessolveComplete() { return m_ressolveComplete; }
	DissolveComplete& OnDissolveComplete() { return m_dissolveComplete; }

	void Show();
	void Hide();

	UFUNCTION(BlueprintCallable)
		void SetDissolveMat(UMaterialInstanceDynamic* mat) { m_dissolveMaterial = mat; }

private:
	UPROPERTY(VisibleAnywhere)
		float visibility = 3.f;

	void SetDissolveDuration(float duration);

	//UFUNCTION()
	void PlayTimeline();
	//UFUNCTION()
	void ReverseTimeline();

	UFUNCTION()
		void TimelineCallback(float val);
	UFUNCTION()
		void TimelineFinishedCallback();

	// Private Variable

	UMaterialInstanceDynamic* m_dissolveMaterial;
	FTimerManager* m_timerMgr;

	RessolveComplete m_ressolveComplete;
	DissolveComplete m_dissolveComplete;

	//Timeline 
	FOnTimelineFloat m_onTimelineCallback;
	FOnTimelineEventStatic m_onTimelineFinishedCallback;

	FTimeline m_timeline;
	UCurveFloat* m_timelineCurve;

};
