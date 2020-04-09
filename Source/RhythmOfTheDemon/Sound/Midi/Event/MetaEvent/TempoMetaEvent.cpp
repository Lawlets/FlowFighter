#include "TempoMetaEvent.h"

#include "../../Chunk/TrackChunk.h"

namespace MIDI
{
	TempoMetaEvent::TempoMetaEvent()
	{
	}

	TempoMetaEvent::TempoMetaEvent(int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
	}

	TempoMetaEvent::~TempoMetaEvent()
	{
	}

	
	void TempoMetaEvent::Init(int deltaTime, ifstream& fileStream, int byteSize)
	{
		MetaEvent::Init(deltaTime, fileStream, byteSize);

		type = MetaEventType::Tempo;

		int totalTempo = ((unsigned char)data[0] << 16) + ((unsigned char)data[1] << 8) + (unsigned char)data[2];
		// convert from microsec per beat to Beat per Minute
		tempo = 60 * 1000000 / totalTempo;
	}

	void TempoMetaEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ***************************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Tempo Meta Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ***************************** "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Type = %i"), (int)type);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Event length = %i"), length);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * New Tempo = %i"), tempo);
		UE_LOG(ApplicationInitLog, Log, TEXT(" ***************************** "));
	}

	void TempoMetaEvent::Apply(TrackChunk* chunk)
	{
		chunk->SetTempo(tempo);
	}
}
