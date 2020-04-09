#include "ChannelEvent.h"

namespace MIDI
{
	ChannelEvent::ChannelEvent()
	{
	}

	ChannelEvent::ChannelEvent(int midiChannel, int deltaTime, ifstream& fileStream, int byteSize)
	{
		channel = midiChannel;

		Init(deltaTime, fileStream, byteSize);
	}

	ChannelEvent::~ChannelEvent()
	{
	}

	void ChannelEvent::Init(int deltaTime, ifstream& fileStream, int byteCount)
	{
		Event::Init(deltaTime, fileStream, byteCount);

		// Need to check how many parameter the event needs

		char str[1];
		byteSize++;
		fileStream.read(str, 1);
		param1 = str[0];

		byteSize++;
		fileStream.read(str, 1);
		param2 = str[0];
	}

	void ChannelEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Channel Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************** "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Channel = %i"), channel);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Parameter 1 = %i"), param1);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Parameter 2 = %i"), param2);
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************** "));
	}
}