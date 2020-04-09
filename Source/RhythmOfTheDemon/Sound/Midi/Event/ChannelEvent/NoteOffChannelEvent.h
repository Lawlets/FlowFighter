#pragma once

#include "ChannelEvent.h"

namespace MIDI
{
	class NoteOffChannelEvent :	public ChannelEvent
	{
	public:
		NoteOffChannelEvent();
		NoteOffChannelEvent(int midiChannel, int deltaTime, ifstream& fileStream, int byteSize);
		~NoteOffChannelEvent();

		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;

		virtual Note GetNote() override { return note; }
		virtual int GetVelocity() override { return param2; }
		virtual int GetOctave() override { return octave; }

	private:
		Note note;
		int octave;
	};
}
