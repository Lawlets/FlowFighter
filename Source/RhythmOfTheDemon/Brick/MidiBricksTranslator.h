// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStruct/BrickData.h"
#include "Sound/Midi/MidiFile.h"
//#include "Sound/Midi/Data/EventEnum.h"

using namespace MIDI;

/**
 * 
 */
class RHYTHMOFTHEDEMON_API MidiBricksTranslator
{
public:
	MidiBricksTranslator();
	~MidiBricksTranslator();

	int GetProjectileCount() { return m_projectileCount; }
	int GetCoopCount() { return m_coopCount; }

	int ComputeTimelinePosition(int deltaTime);

	TArray<FBrickDataStruct> TranslateMidi(FString midiName, TMap<Note, TSubclassOf<AAbstractBrick>> midiBrickMap, int targetOctave);

private:
	MidiFile* m_midiFile;

	int m_projectileCount = 0;
	int m_coopCount = 0;
};
