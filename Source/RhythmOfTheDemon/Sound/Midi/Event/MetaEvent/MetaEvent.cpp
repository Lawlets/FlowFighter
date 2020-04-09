#include "MetaEvent.h"

namespace MIDI
{
	MetaEvent::MetaEvent()
	{
	}

	MetaEvent::MetaEvent(int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
	}

	MetaEvent::~MetaEvent()
	{
	}


	void MetaEvent::Init(int deltaTime, ifstream& fileStream, int byteCount)
	{
		Event::Init(deltaTime, fileStream, byteCount);

		// Data Length
		length = Event::ReadVariableLengthInfo(fileStream, byteSize);

		// Data
		byteSize += length;
		data = new char[length + 1];
		fileStream.read(data, length);
		data[length] = '\0';
	}

	void MetaEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" *********************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Meta Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *********************** "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Type = %i"), (int)type);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Event length = %i"), length);
		if (length > 0)
			UE_LOG(ApplicationInitLog, Log, TEXT(" * Data = %s"), data);
		UE_LOG(ApplicationInitLog, Log, TEXT(" *********************** "));
	}

	void MetaEvent::Apply(TrackChunk* chunk)
	{
		// Do nothing
	}
}