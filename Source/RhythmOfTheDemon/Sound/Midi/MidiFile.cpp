// Fill out your copyright notice in the Description page of Project Settings.

#include "MidiFile.h"

#include "RhythmOfTheDemon.h"
#include "Engine.h"

namespace MIDI
{
	MidiFile::MidiFile()
	{
	}

	MidiFile::MidiFile(FString fileName)
	{
		FString Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) + midiFilePath;
		
		ifstream fileStream;

		fileStream.open(*(Path + fileName), ios::in | ios::binary);

		if (!fileStream.is_open())
			return;

		AnalyzeFile(fileStream);
	}

	MidiFile::~MidiFile()
	{
	}

	void MidiFile::PrintInfo()
	{
		// Print file name ? 
		header.Print();

		for (int i = 0; i < header.GetTrackNumber(); i++)
			tracks[i].Print();
	}

	bool MidiFile::AnalyzeFile(ifstream& fileStream)
	{
		char str[8];

		// Check Midi Header Chunk
		fileStream.read(str, HeaderChunk::MarkerByteSize);
		str[HeaderChunk::MarkerByteSize] = '\0'; // enable string Comparison

		if (strcmp(str, HeaderChunk::ID) != 0)
		{
			UE_LOG(ApplicationInitLog, Log, TEXT("ERROR => First ID of file is not a HeaderChunk ID ... this is not a MIDI file ... Exiting ..."));
			return false;
		}
		open = true;

		header = HeaderChunk(fileStream);
		int trackNb = header.GetTrackNumber();

		// Check Every TrackHeader
		if (trackNb <= 0)
			return true;
		
		tracks = new TrackChunk[trackNb];

		for (int i = 0; i < trackNb; i++)
		{
			fileStream.read(str, TrackChunk::MarkerByteSize);
			str[TrackChunk::MarkerByteSize] = '\0'; // enable string Comparison
			if (strcmp(str, TrackChunk::ID) == 0)
			{
				tracks[i] = TrackChunk(fileStream);
			}
			else
			{
				//Avoid non recognized marker => non recognized Chunk 
				UE_LOG(ApplicationInitLog, Log, TEXT(" => ERROR => Unrecognized Chunk marker found, ignore this chunk for now ..."));
				Chunk unknownChunk(fileStream);
				str[unknownChunk.GetLength()];
				fileStream.read(str, unknownChunk.GetLength());
			}
		}
		return true;
	}

	TrackChunk* MidiFile::GetMainTrack()
	{
		int count = header.GetTrackNumber();
		if (count == 1)
			return &tracks[0];
		else if (count > 1)
			return &tracks[1];

		return nullptr;
	}
}

