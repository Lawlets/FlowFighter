// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
//#include "Engine/EngineTypes.h"

#include "Chunk/HeaderChunk.h"
#include "Chunk/TrackChunk.h"

#include <iostream>
#include <fstream>

using namespace std;


#define MIDI_ID_SIZE 4
#define MIDI_LENGTH_SIZE 4
#define MIDI_FORMAT_SIZE 2
#define MIDI_TRACK_NB_SIZE 2
#define MIDI_TICK_PER_BEAT_SIZE 2

#define MIDI_TRACK_HEADER_SIZE 4

namespace MIDI
{
	
	/**
	 *
	 */
	class RHYTHMOFTHEDEMON_API MidiFile
	{
	public:
		MidiFile();
		MidiFile(FString fileName);
		~MidiFile();

		bool		isOpen() { return open; }
		void		PrintInfo();

		HeaderChunk GetHeader() { return header; }
		TrackChunk* GetMainTrack();

	private:
		bool AnalyzeFile(ifstream& fileStream);

		bool open = false;
		char* midiFilePath = "/FMOD/MIDI/";

		HeaderChunk header;
		TrackChunk* tracks;

		const char MIDI_HEADER[4] = { 'M', 'T', 'h', 'd' };
		const char TRACK_HEADER[4] = { 'M', 'T', 'r', 'k' };
	};
}
