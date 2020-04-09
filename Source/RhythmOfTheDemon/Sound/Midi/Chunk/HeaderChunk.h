#pragma once

#include "Chunk.h"

namespace MIDI
{
	enum /*RHYTHMOFTHEDEMON_API*/ MidiFormat /*: uint8*/
	{
		SingleTrack			/*UMETA(DisplayName = "Single Track")*/,
		MultiTrack			/*UMETA(DisplayName = "Multi Track")*/,
		MultiSong			/*UMETA(DisplayName = "Multi Song")*/,
		None			/*UMETA(DisplayName = "None"),*/
	};

	class HeaderChunk : public Chunk
	{
	public:
		HeaderChunk();
		HeaderChunk(ifstream& fileStream);
		virtual ~HeaderChunk();

		virtual void Print() override;
		virtual void Init(ifstream& fileStream) override;

		int			GetTrackNumber()	{ return tracksCount; }
		MidiFormat	GetFormat()			{ return format; }
		int			GetTicks()			{ return (divisionFlag) ? ticksPerFrame : ticksPerQuarter; }
		int			GetSMPTE()			{ return negativeSMPTEFormat; }


		static const char* ID;

	private:
		MidiFormat	format = MidiFormat::None;
		int			tracksCount = 0;

		bool divisionFlag = false;
		int ticksPerQuarter = -1;
		int ticksPerFrame = -1;
		int negativeSMPTEFormat = 0;


		static const int FormatByteSize = 2;
		static const int TracksCountByteSize = 2;
		static const int DivisionByteSize = 2;
	};
}

