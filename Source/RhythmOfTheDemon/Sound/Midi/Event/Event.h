#pragma once

#include <iostream>
#include <fstream>

#include "../Data/EventEnum.h"
#include "RhythmOfTheDemon.h"
#include "Engine.h"

using namespace std;

namespace MIDI
{
	static int HIGH_BIT = 1 << 7;
	static int HIGH_4_BIT = (1 << 7) | (1 << 6) | (1 << 5) + (1 << 4);
	static int LOW_4_BIT = (1 << 3) | (1 << 2) | (1 << 1) + 1;

	class Event
	{
	public:
		Event();
		Event(int deltaTime, ifstream& fileStream, int byteSize);
		virtual ~Event();

		virtual void Init(int deltaTime, ifstream& fileStream, int byteSize);
		virtual void Print();

		static int ReadVariableLengthInfo(ifstream& fileStream, int& variableLengthByteSize);

		int GetDeltaTime()	{ return deltaTime; }
		int GetByteSize()	{ return byteSize; }

		virtual EventType GetEventType() { return EventType::Unknown; }
		
	protected:
		int deltaTime;
		int byteSize;
	};
}
