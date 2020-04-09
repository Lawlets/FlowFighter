#pragma once
#include "Chunk.h"
#include "../Event/Event.h"
#include "../Data/EventEnum.h"

#include <vector>

namespace MIDI
{
	class TrackChunk : public Chunk
	{
	public:
		TrackChunk();
		TrackChunk(ifstream& fileStream);
		virtual ~TrackChunk();

		virtual void Print() override;
		virtual void Init(ifstream& fileStream) override;

		void ReadEvent(ifstream& fileStream);

		void AddChannelEvent(int type, int channel, int deltaTime, ifstream& fileStream, int byteSize);
		void AddMetaEvent(int deltaTime, ifstream& fileStream, int byteSize);

		static const char* ID;

		string GetName() { return name; }
		void SetName(string value) { name = value; }

		int GetTempo() { return tempo; }
		void SetTempo(int value) { tempo = value; } //TODO : call Event "TempoChange" in addition

		vector<Event*> GetEvents() { return events; }

	private:
		string name = "";
		int tempo = 120; // by default
		vector<Event*> events;

		int timelineDuration = 0;
	};
}

