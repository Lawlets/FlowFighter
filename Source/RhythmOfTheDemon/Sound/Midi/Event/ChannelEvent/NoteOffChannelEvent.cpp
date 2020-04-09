#include "NoteOffChannelEvent.h"


namespace MIDI
{
	NoteOffChannelEvent::NoteOffChannelEvent()
	{
	}

	NoteOffChannelEvent::NoteOffChannelEvent(int midiChannel, int deltaTime, ifstream& fileStream, int byteSize)
	{
		Init(deltaTime, fileStream, byteSize);
		channel = midiChannel;
	}

	NoteOffChannelEvent::~NoteOffChannelEvent()
	{
	}

	void NoteOffChannelEvent::Init(int deltaTime, ifstream& fileStream, int byteSize)
	{
		ChannelEvent::Init(deltaTime, fileStream, byteSize);

		note = (Note)(param1 % 12);
		octave = (param1 / 12) - 1; // range from -1 to 9 
	}

	void NoteOffChannelEvent::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" *************************** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Note Off Event Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *************************** "));
		Event::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Channel = %i"), channel);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Ending Note = %i"), (int)note);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Octave = %i"), octave);
		//UE_LOG(ApplicationInitLog, Log, TEXT(" * Velocity = %i"), param2);
		UE_LOG(ApplicationInitLog, Log, TEXT(" *************************** "));
	}
}