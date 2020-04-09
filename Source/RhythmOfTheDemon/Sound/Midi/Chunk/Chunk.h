#pragma once

#include <iostream>
#include <fstream>

#include "RhythmOfTheDemon.h"
#include "Engine.h"

using namespace std;

namespace MIDI
{
	class Chunk
	{
	public:
		Chunk();
		Chunk(ifstream& fileStream);
		virtual ~Chunk();

		virtual void Print();
		virtual void Init(ifstream& fileStream);

		int GetLength()			{ return length; }

		static const int MarkerByteSize = 4;
		static const int LengthByteSize = 4;

	protected:
		int length;

	};
}
