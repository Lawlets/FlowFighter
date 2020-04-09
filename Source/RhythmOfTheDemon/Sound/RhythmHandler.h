// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DataStruct/LevelDataStruct.h"
#include "RhythmHandler.generated.h"

class AFMODWrapper;

#define InMilisecond(sec) sec * 1000.f
#define InSecond(milisec) milisec / 1000.f

DECLARE_EVENT(ARhythmHandler, OnMusicStart);
DECLARE_EVENT(ARhythmHandler, OnMusicStop);
DECLARE_EVENT(ARhythmHandler, OnMusicEnd);

UCLASS()
class RHYTHMOFTHEDEMON_API ARhythmHandler : public AActor
{
	GENERATED_BODY()
	
private:
	static ARhythmHandler* m_instance;

public:	
	// Sets default values for this actor's properties
	ARhythmHandler();

	UFUNCTION(BlueprintCallable)
	static ARhythmHandler* GetInstance() { return m_instance; }

	UFUNCTION(BlueprintImplementableEvent)
		void MusicStartEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void MusicStopEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Return if the current timeline position is on a beat
	// timeWindow : time allowed between current and beat timeline position ( in Miliseconds )
	bool IsCurrentlyOnBeat(float timeWindow);

	// Return current timeline position (in Miliseconds)
	UFUNCTION(BlueprintCallable)
	int GetTimelinePosition();

	// Return gap in Miliseconds between now and previous beat timeline position 
	int GetPreviousBeatTimelineGap();

	// Return gap in Miliseconds between now and next beat timeline position 
	int GetNextBeatTimelineGap();

	// Return previous beat sound position in Miliseconds 
	int GetPreviousBeatTimelinePosition();

	// Return next beat sound position in Miliseconds
	int GetNextBeatTimelinePosition();

	// Return future beat sound position in Miliseconds
	// int futureBeatNum : num of the beat analyzed 
	int GetFutureBeatTimelinePosition(int futureBeatNum);

	void SetParameter(FName parameterName, float value);
	float GetParameter(FName parameterName);

	void InitAudio(FLevelDataStruct* levelData, bool skipEvent = false);
	void PlayMusic();
	void StopMusic();

	UFUNCTION(BlueprintCallable)
	int GetMusicLength();

	void Pause(bool active);

	bool IsPlaying();

	OnMusicStart& GetMusicStartEvent() { return m_musicStart; }
	OnMusicStop& GetMusicStopEvent() { return m_musicStop; }
	OnMusicEnd& GetMusicEndEvent() { return m_musicEnd; }
	
	FString TimelinePositionToString(int32 timelinePos);

private:

	void UpdateMusicEvent();
	void ClearMusicEvent();
	void LinkMusicEvent();

	OnMusicStart m_musicStart;
	OnMusicStop m_musicStop;
	OnMusicEnd m_musicEnd;


	void OnSoundBeatEvent(int32 Beat, int32 Position);
	void OnMusicStartEvent();
	void OnMusicEndEvent();

	AFMODWrapper* FMODWrap;

	int32 lastBeatTimelinePosition;

};
