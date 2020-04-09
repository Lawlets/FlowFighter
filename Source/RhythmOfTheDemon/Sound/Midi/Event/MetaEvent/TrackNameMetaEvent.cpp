#include "TrackNameMetaEvent.h"

#include "../../Chunk/TrackChunk.h"

namespace MIDI
{
	TrackNameMetaEvent::TrackNameMetaEvent()
	{
	}

	TrackNameMetaEvent::TrackNameMetaEvent(int deltaTime, ifstream& fileStream, int byteSize) 
	{
		Init(deltaTime, fileStream, byteSize);
	}

	TrackNameMetaEvent::~TrackNameMetaEvent()
	{
	}

	void TrackNameMetaEvent::Init(int deltaTime, ifstream& fileStream, int byteSize)
	{
		MetaEvent::Init(deltaTime, fileStream, byteSize);

		type = MetaEventType::TrackName;
		name = string(data);
	}

	void TrackNameMetaEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ********************************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Track Name Meta Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ********************************** "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Type = %i"), (int)type);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Event length = %i"), length);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Track Name = %s"), *FString(data));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ********************************** "));
	}

	void TrackNameMetaEvent::Apply(TrackChunk* chunk)
	{
		chunk->SetName(name);
	}
}
