#pragma once
#include "MetaEvent.h"

namespace MIDI
{
	class TimeSignatureMetaEvent : public MetaEvent
	{
	public:
		TimeSignatureMetaEvent();
		TimeSignatureMetaEvent(int deltaTime, ifstream& fileStream, int byteSize);
		~TimeSignatureMetaEvent();


		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;

		virtual void Apply(TrackChunk* chunk);

	private:
		int numerator;
		int denominator;
		int MIDIClocks;  //(per metronome tick)
		int noteCountPer24Clock; // Number of 1 / 32 notes per 24 MIDI clocks;
	};
}

