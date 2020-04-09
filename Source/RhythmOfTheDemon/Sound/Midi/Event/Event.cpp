#include "Event.h"

namespace MIDI
{
	Event::Event()
	{
	}

	Event::Event(int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
	}

	Event::~Event()
	{
	}


	void Event::Init(int deltaTime, ifstream& fileStream, int byteSize)
	{
		this->deltaTime = deltaTime;
		this->byteSize = byteSize;
	}
	
	void Event::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" * DeltaTime = %i"), deltaTime);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Byte Size = %i"), byteSize);
	}

	int Event::ReadVariableLengthInfo(ifstream& fileStream, int& variableLengthByteSize) // TODO : check, maybe variable length should be compute as Chunk length
	{
		char str[1];
		int value = 0;

		for (int i = 0; i < 4; i++)
		{
			// Read
			variableLengthByteSize++;
			fileStream.read(str, 1);
			int readValue = (unsigned char)str[0];

			// shift previous value
			value <<= 7; 
			// 127 = 0111 1111 , so the value added to res doesn't contain the last bit (flag)
			value += (readValue & 127);				
			//printf("Read Variable = %i\n", readValue);

			int LastBit = (readValue & HIGH_BIT);
			//printf("Last bit = %i\n", LastBit);
			// if last byte has not been set, end Reading
			if (!LastBit)
				break;
		}
		return value;
	}
}
