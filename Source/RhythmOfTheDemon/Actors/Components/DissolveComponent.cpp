// Fill out your copyright notice in the Description page of Project Settings.

#include "DissolveComponent.h"
#include "ConstructorHelpers.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

// Sets default values for this component's properties
UDissolveComponent::UDissolveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.Target = this;

	ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Curve/LinearCurve"));
	check(Curve.Succeeded());

	m_timelineCurve = Curve.Object;
	m_timeline = FTimeline();
}


// Called when the game starts
void UDissolveComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.SetTickFunctionEnable(true);

	if (m_timelineCurve != NULL)
	{
		m_timeline.SetFloatCurve(m_timelineCurve, FName("linearCurve"));
		m_timeline.SetLooping(false);
		m_timeline.SetTimelineLength(1.f);
		m_timeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		m_timeline.SetPlaybackPosition(0.0f, false);

		m_onTimelineCallback.BindUFunction(this, FName("TimelineCallback") );
		m_onTimelineFinishedCallback.BindUFunction(this, FName("TimelineFinishedCallback"));
		m_timeline.AddInterpFloat(m_timelineCurve, m_onTimelineCallback);
		m_timeline.SetTimelineFinishedFunc(m_onTimelineFinishedCallback);
	}
}

void UDissolveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(ApplicationInitLog, Log, TEXT(" Dissolve Component Tick !"));
	m_timeline.TickTimeline(DeltaTime);
}

void UDissolveComponent::Init(UMaterialInstanceDynamic* dissolveMaterial, FTimerManager* timer)
{
	m_dissolveMaterial = dissolveMaterial;
	

	m_timerMgr = timer;
}


void UDissolveComponent::Ressolve(float duration)
{
	SetDissolveDuration(duration);
	m_timeline.Play();
}

void UDissolveComponent::Ressolve(float duration, float delay)
{
	if (delay <= 0.f)
	{
		Ressolve(duration);
		return;
	}

	SetDissolveDuration(duration);
	
	FTimerHandle UnusedHandle;
	m_timerMgr->SetTimer(UnusedHandle, this, &UDissolveComponent::PlayTimeline, delay);
}

void UDissolveComponent::Dissolve(float duration)
{
	SetDissolveDuration(duration);
	m_timeline.Reverse();
}

void UDissolveComponent::Dissolve(float duration, float delay)
{
	if (delay <= 0.f)
	{
		Dissolve(duration);
		return;
	}

	SetDissolveDuration(duration);
	
	FTimerHandle UnusedHandle;
	m_timerMgr->SetTimer(UnusedHandle, this, &UDissolveComponent::ReverseTimeline, delay);
}

void UDissolveComponent::Show()
{
	m_dissolveMaterial->SetScalarParameterValue(FName("Dissolve"), 1.f);
	m_timeline.SetPlaybackPosition(1.f, false);
}

void UDissolveComponent::Hide()
{
	m_dissolveMaterial->SetScalarParameterValue(FName("Dissolve"), 0.f);
	m_timeline.SetPlaybackPosition(0.f, false);
}

//Private 

void UDissolveComponent::SetDissolveDuration(float duration)
{
	m_timeline.SetPlayRate(1.f / duration);
}

void UDissolveComponent::PlayTimeline()
{
	m_timeline.Play();
}

void UDissolveComponent::ReverseTimeline()
{
	m_timeline.Reverse();
}

void UDissolveComponent::TimelineCallback(float val)
{
	//UE_LOG(ApplicationInitLog, Log, TEXT(" timeline Callback with %f percent"), val);
	m_dissolveMaterial->SetScalarParameterValue(FName("Dissolve"), val);
}

void UDissolveComponent::TimelineFinishedCallback()
{
	float currPosition = m_timeline.GetPlaybackPosition();

	if (currPosition >= m_timeline.GetTimelineLength())
	{
		//UE_LOG(ApplicationInitLog, Log, TEXT(" Resolve Complete !"));
		m_ressolveComplete.Broadcast();
	}
	else if (currPosition <= 0.f)
	{
		//UE_LOG(ApplicationInitLog, Log, TEXT(" Dissolve Complete !"));
		m_dissolveComplete.Broadcast();
	}
}
