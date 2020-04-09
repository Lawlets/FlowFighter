#pragma once

#include "../Event.h"
#include "../../Data/EventEnum.h"

namespace MIDI
{
	class TrackChunk;

	/*enum MetaApplication
	{
		None = -1,
		Whenever,
		AtTimePosition,
	};*/

	class MetaEvent : public Event
	{
	public:
		MetaEvent();
		MetaEvent(int deltaTime, ifstream& fileStream, int byteSize);
		virtual ~MetaEvent();

		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;
		virtual EventType GetEventType() override { return EventType::Meta; }

		virtual void Apply(TrackChunk* chunk);


		MetaEventType	GetType()			{ return type; }
		//MetaApplication GetApplication()	{ return application; }
		int				GetLength()			{ return length; }

	protected:
		MetaEventType type = MetaEventType::Unknown;
		int length = -1;

		//MetaApplication application = MetaApplication::None;
		char* data = nullptr;
	};
}
