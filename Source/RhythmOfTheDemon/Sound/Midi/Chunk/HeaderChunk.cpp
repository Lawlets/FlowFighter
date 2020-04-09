#include "HeaderChunk.h"

namespace MIDI
{
	const char* HeaderChunk::ID = "MThd";

	HeaderChunk::HeaderChunk()
	{
	}

	HeaderChunk::HeaderChunk(ifstream& fileStream)
	{
		Init(fileStream);
	}

	HeaderChunk::~HeaderChunk()
	{
	}

	void HeaderChunk::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************* "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Header Chunk Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************* "));
		Chunk::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Format = %i"), (int)format);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Number of Tracks = %i"), tracksCount);
		if (divisionFlag)
		{
			UE_LOG(ApplicationInitLog, Log, TEXT(" * Ticks per Frame = %i"), ticksPerFrame);
			UE_LOG(ApplicationInitLog, Log, TEXT(" * Negative SMPTE format = %i"), negativeSMPTEFormat);
		}
		else
			UE_LOG(ApplicationInitLog, Log, TEXT(" * Ticks per Quarter note = %i"), ticksPerQuarter);
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************* "));
	}

	void HeaderChunk::Init(ifstream& fileStream)
	{
		Chunk::Init(fileStream);

		char str[8];

		fileStream.read(str, FormatByteSize);
		int intFormat = 0;
		for (int i = 0; i < FormatByteSize; i++)
			intFormat += str[i];
		format = (MidiFormat)intFormat;

		// Number of Track(s)
		fileStream.read(str, TracksCountByteSize);
		for (int i = 0; i < TracksCountByteSize; i++)
			tracksCount += (unsigned char)str[i];

		// Number of Ticks per Beat
		int division = 0;
		fileStream.read(str, DivisionByteSize);
		for (int i = 0; i < DivisionByteSize; i++)
			division += (unsigned char)str[i];

		int divisionLastBit = (division & (1 << 15));
		divisionFlag = divisionLastBit >> 15;

		if (divisionFlag)
		{
			printf("division with flag has not been tested yet ... ");
			ticksPerFrame = (division & 255); // 255 = 1111 1111 => 8 set bit from 0 to 7 
			negativeSMPTEFormat = (division & (127 << 8)) >> 8; // 127 = 0111 1111 => 7 set bit 0 to 6
		}
		else
			ticksPerQuarter = division - divisionLastBit;

		// TODO : Upgrade Reader to avoid forcing any value into it :x
		ticksPerQuarter = 960;
	}
}
