// Fill out your copyright notice in the Description page of Project Settings.

#include "RhythmHandler.h"

#include "RhythmOfTheDemon.h"
#include "Engine.h"
#include "FMODWrapper.h"
#include "Midi/MidiFile.h"

#include "RhythmOfTheDemonGameModeBase.h"
#include "RhythmOfTheDemonGameStateBase.h"
//#include "DataStruct/LevelDataStruct.h"


ARhythmHandler* ARhythmHandler::m_instance;

// Sets default values
ARhythmHandler::ARhythmHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_instance = this;
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponentRoot");
}

// Called when the game starts or when spawned
void ARhythmHandler::BeginPlay()
{
	Super::BeginPlay();

	FMODWrap = GetWorld()->SpawnActor<AFMODWrapper>();
	FMODWrap->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	FMODWrap->OnSoundBeat.AddUObject(this, &ARhythmHandler::OnSoundBeatEvent);
}

// Called every frame
void ARhythmHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARhythmHandler::UpdateMusicEvent()
{
	ClearMusicEvent();
	LinkMusicEvent();
}

void ARhythmHandler::ClearMusicEvent()
{
	FMODWrap->OnMusicStartEvent.Clear();
	FMODWrap->OnMusicStartEvent.RemoveAll(this);
	
	FMODWrap->OnMusicEndEvent.Clear();
	FMODWrap->OnMusicEndEvent.RemoveAll(this);

}

int ARhythmHandler::GetMusicLength()
{
	return FMODWrap->GetEventTimeLength();
}

void ARhythmHandler::LinkMusicEvent()
{
	FMODWrap->OnMusicStartEvent.AddUObject(this, &ARhythmHandler::OnMusicStartEvent);
	FMODWrap->OnMusicEndEvent.AddUObject(this, &ARhythmHandler::OnMusicEndEvent);
}

void ARhythmHandler::InitAudio(FLevelDataStruct* levelData, bool skipEvent)
{
	if(skipEvent == false)
		UpdateMusicEvent();
	FMODWrap->InitAudioFromLevelData(levelData);
}

void ARhythmHandler::SetParameter(FName parameterName, float value)
{
	FMODWrap->SetParameter(parameterName, value);
}

float ARhythmHandler::GetParameter(FName parameterName)
{
	return FMODWrap->GetParameter(parameterName);
}

void ARhythmHandler::PlayMusic()
{
	FMODWrap->Play();
	MusicStartEvent();
}

void ARhythmHandler::StopMusic()
{
	ClearMusicEvent();
	FMODWrap->Stop();
	FMODWrap->SetParameter("Skill", 2); // Reset Parameter
	MusicStopEvent();
	//LinkMusicEvent();
}

bool ARhythmHandler::IsPlaying()
{
	return FMODWrap->IsPlaying();
}

void ARhythmHandler::OnMusicStartEvent()
{
	m_musicStart.Broadcast();
}

void ARhythmHandler::OnMusicEndEvent()
{	

	ARhythmOfTheDemonGameStateBase* gameState = GetWorld()->GetGameState<ARhythmOfTheDemonGameStateBase>();

	if (gameState)
		gameState->OnLevelEnd();

	m_musicEnd.Broadcast();
}

void ARhythmHandler::Pause(bool active)
{
	FMODWrap->Pause(active);

	if (active)
		MusicStopEvent();
	else
		MusicStartEvent();
}

bool ARhythmHandler::IsCurrentlyOnBeat(float timeWindow)
{
	int previousBeatPos = GetPreviousBeatTimelineGap();
	//UE_LOG(ApplicationInitLog, Log, TEXT("Previous beat gap is %i"), previousBeatPos);
	if (previousBeatPos <= timeWindow)
		return true;

	int nextBeatPos = GetNextBeatTimelineGap();
	//UE_LOG(ApplicationInitLog, Log, TEXT("Next beat gap is %i"), nextBeatPos);
	if (nextBeatPos <= timeWindow)
		return true;

	return false;
}

int ARhythmHandler::GetTimelinePosition()
{
	return FMODWrap->GetTimelinePosition();
}

int ARhythmHandler::GetPreviousBeatTimelineGap()
{
	int currTimelinePosition = FMODWrap->GetTimelinePosition();

	return currTimelinePosition - lastBeatTimelinePosition;
}

int ARhythmHandler::GetNextBeatTimelineGap()
{
	int currTimelinePosition = FMODWrap->GetTimelinePosition();
	int nextBeatTimelinePosition = GetNextBeatTimelinePosition();

	return nextBeatTimelinePosition - currTimelinePosition;
}

int ARhythmHandler::GetPreviousBeatTimelinePosition()
{
	return lastBeatTimelinePosition;
}

int ARhythmHandler::GetNextBeatTimelinePosition()
{
	return GetFutureBeatTimelinePosition(1);
}

int ARhythmHandler::GetFutureBeatTimelinePosition(int futureBeatNum)
{
	return lastBeatTimelinePosition + (FMODWrap->GetBeatGap() * futureBeatNum);
}

FString ARhythmHandler::TimelinePositionToString(int32 timelinePos)
{
	return FString::FromInt(timelinePos / 1000) + "s " + FString::FromInt(timelinePos % 1000) + "ms";
}

void ARhythmHandler::OnSoundBeatEvent(int32 Beat, int32 Position)
{
	lastBeatTimelinePosition = Position;
	//UE_LOG(ApplicationInitLog, Log, TEXT("Last Beat was heared at %i"), lastBeatTimelinePosition);
}




