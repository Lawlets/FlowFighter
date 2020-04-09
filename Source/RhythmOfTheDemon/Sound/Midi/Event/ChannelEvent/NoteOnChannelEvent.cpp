#include "NoteOnChannelEvent.h"

namespace MIDI
{
	NoteOnChannelEvent::NoteOnChannelEvent()
	{
	}

	NoteOnChannelEvent::NoteOnChannelEvent(int midiChannel, int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
		channel = midiChannel;
	}

	NoteOnChannelEvent::~NoteOnChannelEvent()
	{
	}

	void NoteOnChannelEvent::Init(int deltaTime, ifstream& fileStream, int byteSize)
	{
		ChannelEvent::Init(deltaTime, fileStream, byteSize);

		note = (Note)(param1 % 12);
		octave = (param1 / 12) - 1; // range from -1 to 9 
	}

	void NoteOnChannelEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Note On Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************** "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Channel = %i"), channel);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * New Note = %i"), (int)note);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Octave = %i"), octave);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Velocity = %i"), param2);
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************** "));
	}
}
