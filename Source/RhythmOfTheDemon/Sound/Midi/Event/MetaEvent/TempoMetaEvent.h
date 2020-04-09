#pragma once
#include "MetaEvent.h"

namespace MIDI
{
	class TempoMetaEvent : public MetaEvent
	{
	public:
		TempoMetaEvent();
		TempoMetaEvent(int deltaTime, ifstream& fileStream, int byteSize);
		~TempoMetaEvent();

		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;

		virtual void Apply(TrackChunk* chunk) override;

	private:
		int tempo;
	};
}

