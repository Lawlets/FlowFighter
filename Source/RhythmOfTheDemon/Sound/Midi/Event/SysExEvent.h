#pragma once
#include "Event.h"

namespace MIDI
{
	class SysExEvent : public Event
	{
	public:
		SysExEvent();
		SysExEvent(int deltaTime, ifstream& fileStream, int byteSize);
		virtual ~SysExEvent();

		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;
		virtual EventType GetEventType() override { return EventType::SystemEx; }

		int GetLength() { return length; }


	private:
		int length;

		char* data;
	};
}