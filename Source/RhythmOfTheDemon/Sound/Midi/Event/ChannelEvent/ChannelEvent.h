#pragma once

#include "../Event.h"
#include "../../Data/EventEnum.h"

namespace MIDI
{
	class ChannelEvent : public Event
	{
	public:
		ChannelEvent();
		ChannelEvent(int midiChannel, int deltaTime, ifstream& fileStream, int byteSize);
		virtual ~ChannelEvent();

		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;
		virtual EventType GetEventType() override { return EventType::Channel; }

		virtual Note GetNote() { return Note::None; }
		virtual int GetVelocity() { return 0; }
		virtual int GetOctave() { return -1; }

		void SetChannel(int newChannel) { channel = newChannel; }
		int GetChannel() { return channel; }

	protected:
		ChannelEventType type = ChannelEventType::Unknown;
		int channel = -1;

		int param1 = -1;
		int param2 = -1;
	};
}