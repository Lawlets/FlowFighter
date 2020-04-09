#pragma once
#include "MetaEvent.h"

namespace MIDI
{
	class EndTrackMetaEvent : public MetaEvent
	{
	public:
		EndTrackMetaEvent();
		EndTrackMetaEvent(int deltaTime, ifstream& fileStream, int byteSize);
		~EndTrackMetaEvent();


		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;

		virtual void Apply(TrackChunk* chunk) override;
	};
}

