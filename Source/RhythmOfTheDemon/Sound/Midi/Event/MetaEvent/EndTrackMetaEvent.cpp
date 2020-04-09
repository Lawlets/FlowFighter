#include "EndTrackMetaEvent.h"

#include "../../Chunk/TrackChunk.h"

namespace MIDI
{
	EndTrackMetaEvent::EndTrackMetaEvent()
	{
	}

	EndTrackMetaEvent::EndTrackMetaEvent(int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
	}

	EndTrackMetaEvent::~EndTrackMetaEvent()
	{
	}

	void EndTrackMetaEvent::Init(int deltaTime, ifstream& fileStream, int byteSize)
	{
		MetaEvent::Init(deltaTime, fileStream, byteSize);

		type = MetaEventType::EndOfTrack;
	}

	void EndTrackMetaEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************************ "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** End of Track Meta Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************************ "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Type = %i"), (int)type);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Event length = %i"), length);
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************************ "));
	}

	void EndTrackMetaEvent::Apply(TrackChunk* chunk)
	{
		// TODO : Call End Event/Function
	}
}
