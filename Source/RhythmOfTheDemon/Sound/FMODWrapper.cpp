// Fill out your copyright notice in the Description page of Project Settings.

#include "FMODWrapper.h"
#include "RhythmOfTheDemon.h"
#include "Engine.h"

//#include "FMODEvent.h"
#include "FMODAudioComponent.h"
#include "DataStruct/LevelDataStruct.h"

// Sets default values
AFMODWrapper::AFMODWrapper()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	FMODAudio = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FMODAudioComp"));
	if (!FMODAudio)
	{
		UE_LOG(ApplicationInitLog, Log, TEXT("FMODAudioComp has not been created !"));
		return;
	}
	AddOwnedComponent(FMODAudio);
	FMODAudio->bEnableTimelineCallbacks = true;
}

// Called when the game starts or when spawned
void AFMODWrapper::BeginPlay()
{
	Super::BeginPlay();
	FMODAudio->OnEventStopped.__Internal_AddDynamic(this, &AFMODWrapper::OnMusicEnd, "OnMusicEnd");
	FMODAudio->OnTimelineBeat.__Internal_AddDynamic(this, &AFMODWrapper::OnBeat, "OnBeat");
}

// Called every frame
void AFMODWrapper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFMODWrapper::InitAudioFromLevelData(FLevelDataStruct* levelData)
{
	//UE_LOG(ApplicationInitLog, Log, TEXT("Init FMODInterface with following data ->"));
	//UE_LOG(ApplicationInitLog, Log, TEXT("* Name is %s"), *levelData->Name);
	//UE_LOG(ApplicationInitLog, Log, TEXT("* BPM is %i"), levelData->BPM);
	//UE_LOG(ApplicationInitLog, Log, TEXT("* Sound Event name is %s"), *levelData->soundEvent->FileName);

	/*tempo = levelData->BPM;*/
	FMODAudio->SetEvent(levelData->soundEvent);
}

void AFMODWrapper::SetParameter(FName parameterName, float value)
{
	FMODAudio->SetParameter(parameterName, value);
}

float AFMODWrapper::GetParameter(FName parameterName)
{
	return FMODAudio->GetParameter(parameterName);
}

void AFMODWrapper::Play()
{
	if (FMODAudio->Event)
	{
		FMODAudio->Play();
		Pause(false);
		OnMusicStartEvent.Broadcast();
	}
}

void AFMODWrapper::Pause(bool active)
{
	if (FMODAudio->Event)
	{
		isPaused = active;
		FMODAudio->SetPaused(active);
	}
}

void AFMODWrapper::Stop()
{
	if (FMODAudio->Event)
	{
		FMODAudio->Release();
		FMODAudio->Stop();
	}
}

bool AFMODWrapper::IsPlaying()
{
	if (isPaused)
		return false;
	else 
		return FMODAudio->IsPlaying();
}

int AFMODWrapper::GetTimelinePosition()
{
	return FMODAudio->GetTimelinePosition();
}

int AFMODWrapper::GetEventTimeLength()
{
	return FMODAudio->GetLength();
}

int AFMODWrapper::GetBPM()
{
	return tempo;
}

float AFMODWrapper::GetBeatGap()
{
	// TODO : Maybe avoid recalculating it every time
	float beatPerSec = (float)(tempo) / 60.f;
	return (1.f / beatPerSec) * 1000.f;
}

void AFMODWrapper::OnBeat(int32 Bar, int32 Beat, int32 Position, float Tempo, int32 TimeSignatureUpper, int32 TimeSignatureLower)
{
	tempo = Tempo;

	//UE_LOG(ApplicationInitLog, Log, TEXT(" -> Beat <- "));
	//UE_LOG(ApplicationInitLog, Log, TEXT(" * Bar =  %i"), Bar);
	//UE_LOG(ApplicationInitLog, Log, TEXT(" * Beat = %i"), Beat);
	//UE_LOG(ApplicationInitLog, Log, TEXT(" * Position =  %i"), Position);
	//UE_LOG(ApplicationInitLog, Log, TEXT(" * Tempo =  %f"), Tempo);
	/*UE_LOG(ApplicationInitLog, Log, TEXT(" * TimeSignatureUpper =  %i"), TimeSignatureUpper);
	UE_LOG(ApplicationInitLog, Log, TEXT(" * TimeSignatureLower =  %i"), TimeSignatureLower);*/
	
	if (OnSoundBeat.IsBound())
		OnSoundBeat.Broadcast(Beat, Position);
}

void AFMODWrapper::OnMusicEnd()
{
	if (OnMusicEndEvent.IsBound())
		OnMusicEndEvent.Broadcast();
}
