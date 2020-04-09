#include "TimeSignatureMetaEvent.h"

#include "../../Chunk/TrackChunk.h"

namespace MIDI
{
	TimeSignatureMetaEvent::TimeSignatureMetaEvent()
	{
	}

	TimeSignatureMetaEvent::TimeSignatureMetaEvent(int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
		
	}

	TimeSignatureMetaEvent::~TimeSignatureMetaEvent()
	{
	}

	void TimeSignatureMetaEvent::Init(int deltaTime, ifstream& fileStream, int byteSize)
	{
		MetaEvent::Init(deltaTime, fileStream, byteSize);

		type = MetaEventType::TimeSignature;

		numerator = (unsigned char)data[0];
		denominator = (unsigned char)data[1];
		MIDIClocks = (unsigned char)data[2];
		noteCountPer24Clock = (unsigned char)data[3];
	}

	void TimeSignatureMetaEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Time Signature Meta Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************************** "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Type = %i"), (int)type);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Event length = %i"), length);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Numerator = %i"), numerator);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Denominator = %i"), denominator);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * MidiClocks per ticks = %i"), MIDIClocks);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Note count per 24 clocks = %i"), noteCountPer24Clock);
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************************** "));
	}

	void TimeSignatureMetaEvent::Apply(TrackChunk* chunk)
	{
		//TODO : Understand what all this time signature/clock is and how to use it ... 
	}
}
