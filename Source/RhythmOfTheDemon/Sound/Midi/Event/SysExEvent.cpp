#include "SysExEvent.h"

namespace MIDI
{
	SysExEvent::SysExEvent()
	{
	}

	SysExEvent::SysExEvent(int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
	}

	SysExEvent::~SysExEvent()
	{
	}


	void SysExEvent::Init(int deltaTime, ifstream& fileStream, int byteCount)
	{
		Event::Init(deltaTime, fileStream, byteCount);

		length = Event::ReadVariableLengthInfo(fileStream, byteSize);

		byteSize += length;
		
		data = new char[length + 1];
		fileStream.read(data, length);
		data[length] = '\0';
	}

	void SysExEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ********************************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** System Exlusive Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ********************************** "));
		Event::Print();
		//UE_LOG(ApplicationInitLog, Log, TEXT(" * Type = %i"), type);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Length = %i"), length);
		/*if (length > 0)
			UE_LOG(ApplicationInitLog, Log, TEXT(" * Data = %s"), data);*/
		UE_LOG(ApplicationInitLog, Log, TEXT(" ********************************** "));
	}
}