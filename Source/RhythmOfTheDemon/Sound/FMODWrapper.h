// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "SoundWrapper.h"

#include "Event.h"
#include "GameFramework/Actor.h"
#include "FMODWrapper.generated.h"

struct FLevelDataStruct;
class UFMODAudioComponent;

DECLARE_EVENT_TwoParams(AFMODWrapper, FOnSoundBeat, int32, int32);
DECLARE_EVENT(ARhythmHandler, FOnMusicStart);
DECLARE_EVENT(ARhythmHandler, FOnMusicEnd);

UCLASS()
class RHYTHMOFTHEDEMON_API AFMODWrapper : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFMODWrapper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Load every usefull sound information from leveldata
	void InitAudioFromLevelData(FLevelDataStruct* levelData);

	void SetParameter(FName parameterName, float value);
	float GetParameter(FName parameterName);

	// Play music depending of previous leveldata initialized
	void Play();
	void Pause(bool active);
	void Stop();

	// Return number of Beat per Minuts, also called Tempo
	int GetTimelinePosition();

	// Return length of current event in milisec
	int GetEventTimeLength();

	// Return number of Beat per Minuts, also called Tempo
	int GetBPM();

	// Return time between each beat in Miliseconds
	float GetBeatGap();

	bool IsPlaying();

	FOnSoundBeat OnSoundBeat;
	FOnMusicStart OnMusicStartEvent;
	FOnMusicEnd OnMusicEndEvent;

private:
	UFUNCTION()
		void OnBeat(int32 Bar, int32 Beat, int32 Position, float Tempo, int32 TimeSignatureUpper, int32 TimeSignatureLower);

	UFUNCTION()
		void OnMusicEnd();

	UFMODAudioComponent* FMODAudio;
	int tempo;

	bool isPaused;
};
