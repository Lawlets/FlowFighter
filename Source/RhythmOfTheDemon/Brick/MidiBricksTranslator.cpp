// Fill out your copyright notice in the Description page of Project Settings.

#include "MidiBricksTranslator.h"

#include "Sound/Midi/Data/EventEnum.h"
#include "Sound/Midi/Chunk/TrackChunk.h"
#include "Sound/Midi/Event/ChannelEvent/ChannelEvent.h"
#include "Brick/AbstractProjectile.h"

using namespace MIDI;

MidiBricksTranslator::MidiBricksTranslator()
{

}

MidiBricksTranslator::~MidiBricksTranslator()
{
}

int MidiBricksTranslator::ComputeTimelinePosition(int deltaTime)
{
	if (!m_midiFile)
	{
		UE_LOG(ApplicationInitLog, Log, TEXT("*Cannot Compute TimelinePosition"));
		return 0;
	}

	float tickPerQuarterNote = m_midiFile->GetHeader().GetTicks(); // Hard set to 960 for reaper usage
	float quarterNote = deltaTime / tickPerQuarterNote;

	float BPM = m_midiFile->GetMainTrack()->GetTempo();
	float BPS = BPM / 60; //convert into second

	return (quarterNote / BPS) * 1000; // Switch to Milisecond
}

TArray<FBrickDataStruct> MidiBricksTranslator::TranslateMidi(FString midiName, TMap<Note, TSubclassOf<AAbstractBrick>> midiBrickMap, int targetOctave)
{
	m_projectileCount = 0;
	m_coopCount = 0;
	TArray<FBrickDataStruct> brickArray = TArray<FBrickDataStruct>();

//	UE_LOG(ApplicationInitLog, Log, TEXT("* Start Midi Translation"));

	m_midiFile = new MidiFile(midiName);
	//m_midiFile->PrintInfo();

	TrackChunk* chunk = m_midiFile->GetMainTrack();

	vector<Event*> events = chunk->GetEvents();
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i]->GetEventType() == EventType::Channel) // Channel Event only
		{
			ChannelEvent* chanEvent = (ChannelEvent*)(events[i]);
			if (!chanEvent)
				continue;

			if (chanEvent->GetVelocity() > 0) // Avoid Note off
			{
				Note newNote = chanEvent->GetNote();
				if (midiBrickMap.Contains(newNote)) // Avoid Non Note Event
				{
					//UE_LOG(ApplicationInitLog, Log, TEXT("* New Note is = %i"), (int)newNote);
					//UE_LOG(ApplicationInitLog, Log, TEXT("* New Note Octave is = %i"), chanEvent->GetOctave());
					//UE_LOG(ApplicationInitLog, Log, TEXT("* Targeted Octave is = %i"), targetOctave);
					if (chanEvent->GetOctave() != targetOctave) // Ignore every note that is not on the targeted octave
						continue;

					FBrickDataStruct newBrick;
					newBrick.brickClass = midiBrickMap[newNote];
					newBrick.TimelinePosition = ComputeTimelinePosition(chanEvent->GetDeltaTime());
					//UE_LOG(ApplicationInitLog, Log, TEXT("* New Note with delta time = %i"), chanEvent->GetDeltaTime());
//					UE_LOG(ApplicationInitLog, Log, TEXT("* New Note with position = %i"), newBrick.TimelinePosition);
					if (newBrick.brickClass)
						brickArray.Add(newBrick);

					// Check if note is a Projectile Note
					if (newNote == Note::Do || newNote == Note::DoDiese || newNote == Note::Mi || newNote == Note::ReDiese)
					{
						//					UE_LOG(ApplicationInitLog, Log, TEXT("* New Projectile"));
						m_projectileCount++;
					}
					else if (newNote == Note::FaDiese)
						m_coopCount++;
				}
				else
					UE_LOG(ApplicationInitLog, Log, TEXT("* New Note not contain in midiBrickMap"));
			}
		}
	}
	UE_LOG(ApplicationInitLog, Log, TEXT("* brick Array size = %i"), brickArray.Num());

	UE_LOG(ApplicationInitLog, Log, TEXT("* => projectile Count = %i"), m_projectileCount);
	UE_LOG(ApplicationInitLog, Log, TEXT("* => coop Count = %i"), m_coopCount);

	return brickArray;
}
