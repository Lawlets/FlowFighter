#pragma once
#include "MetaEvent.h"

namespace MIDI
{
	class TrackNameMetaEvent : public MetaEvent
	{
	public:
		TrackNameMetaEvent();
		TrackNameMetaEvent(int deltaTime, ifstream& fileStream, int byteSize);
		~TrackNameMetaEvent();

		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize) override;
		virtual void Print() override;

		virtual void Apply(TrackChunk* chunk) override;

	private:
		string name = "";
	};
}
