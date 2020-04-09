#include "Chunk.h"

namespace MIDI
{
	Chunk::Chunk()
	{
	}

	Chunk::Chunk(ifstream& fileStream)
	{
		Init(fileStream);
	}

	Chunk::~Chunk()
	{
	}

	void Chunk::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT("* Chunk size = %i"), length);
	}

	void Chunk::Init(ifstream& fileStream)
	{
		char str[4];

		fileStream.read(str, LengthByteSize);
		int first = (unsigned char)str[0] << (8 * 3);
		int sec = (unsigned char)str[1] << (8 * 2);
		int third = (unsigned char)str[2] << 8;
		int fourth = (unsigned char)str[3];

		length = first + sec + third + fourth;
	}
}
