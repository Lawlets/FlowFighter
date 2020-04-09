#include "TrackChunk.h"

#include "../Event/SysExEvent.h"

// Meta Event Includes
#include "../Event/MetaEvent/TrackNameMetaEvent.h"
#include "../Event/MetaEvent/TempoMetaEvent.h"
#include "../Event/MetaEvent/TimeSignatureMetaEvent.h"
#include "../Event/MetaEvent/EndTrackMetaEvent.h"

// Channel Event Includes
#include "../Event/ChannelEvent/NoteOffChannelEvent.h"
#include "../Event/ChannelEvent/NoteOnChannelEvent.h"


namespace MIDI
{
	const char* TrackChunk::ID = "MTrk";

	TrackChunk::TrackChunk()
	{
	}

	TrackChunk::TrackChunk(ifstream& fileStream)
	{
		Init(fileStream);
	}

	TrackChunk::~TrackChunk()
	{
	}

	void TrackChunk::Print()
	{
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************ "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" *** Track Chunk Info *** "));
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************ "));
		if (name != "")
			UE_LOG(ApplicationInitLog, Log, TEXT(" * Name  = %s"), *FString(name.c_str()));
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Tempo  = %i"), tempo);
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Duration = %i"), timelineDuration);
		Chunk::Print();
		UE_LOG(ApplicationInitLog, Log, TEXT(" * Number of Event  = %i"), (int)events.size());
		UE_LOG(ApplicationInitLog, Log, TEXT(" ************************ "));

		for (size_t i = 0; i < events.size(); i++)
			events[i]->Print();
	}

	void TrackChunk::Init(ifstream& fileStream)
	{
		Chunk::Init(fileStream);

		int totalEventSize = 0;
		//Loop it
		while (totalEventSize < length)
		{
			ReadEvent(fileStream);
			totalEventSize += events.back()->GetByteSize();
		}
	}

	void TrackChunk::ReadEvent(ifstream& fileStream)
	{
		char str[32];
		int byteSize = 0;

		// DeltaTime
		int deltaTime = Event::ReadVariableLengthInfo(fileStream, byteSize);
		timelineDuration += deltaTime;

		// Event Type
		byteSize++;
		fileStream.read(str, 1);

		int eventType = (unsigned char)str[0];
		int top4BitEventType = (unsigned char)(eventType & HIGH_4_BIT) >> 4;

		// Meta Event => 255
		if (eventType == 0xFF)
		{
			AddMetaEvent(timelineDuration, fileStream, byteSize);
		}
		// Channel Event => 4 high bit => from 8 to 14 (inclusive)
		else if (top4BitEventType >= 0x8 && top4BitEventType <= 0xE)
		{
			int channel = (unsigned char)(eventType & LOW_4_BIT);
			AddChannelEvent(top4BitEventType, channel, timelineDuration, fileStream, byteSize);
		}
		// System Exclusive Event => 240 or 247
		else if (eventType == 0xF0 || eventType == 0xF7) // either 240 or 247
		{
			SysExEvent* sysEvent = new SysExEvent(timelineDuration, fileStream, byteSize);
			events.emplace_back(sysEvent);
		}
		// Unknown Event => every other value
		else
		{
			UE_LOG(ApplicationInitLog, Log, TEXT(" => It's an Unknown event, type = %i"), eventType);
			Event* unknownEvent = new Event(timelineDuration, fileStream, byteSize);
			events.emplace_back(unknownEvent);
		}

		//events.back()->Print();
	}

	void TrackChunk::AddChannelEvent(int type, int channel, int deltaTime, ifstream& fileStream, int byteSize)
	{
		ChannelEventType eventType = (ChannelEventType)type;
		ChannelEvent* channelEvent;

		switch (eventType)
		{
			/*case ChannelEventType::Unknown:
				break;*/
		case ChannelEventType::NoteOff:
			channelEvent = new NoteOffChannelEvent(channel, deltaTime, fileStream, byteSize);
			break;
		case ChannelEventType::NoteOn:
			channelEvent = new NoteOnChannelEvent(channel, deltaTime, fileStream, byteSize);
			break;
			/*case ChannelEventType::NoteAftertouch:
				break;*/
				/*case ChannelEventType::Controller:
					break;*/
					/*case ChannelEventType::ProgramChange:
						break;*/
						/*case ChannelEventType::ChannelAfterTouch:
							break;*/
							/*case ChannelEventType::PitchBend:
								//break;*/
		default:
			channelEvent = new ChannelEvent(channel, deltaTime, fileStream, byteSize);
			break;
		}
		events.emplace_back(channelEvent);
	}

	void TrackChunk::AddMetaEvent(int deltaTime, ifstream& fileStream, int byteSize)
	{
		//printf("It's a MetaEvent !\n");
		char str[1];
		fileStream.read(str, 1);
		byteSize++;

		MetaEventType type = (MetaEventType)((unsigned char)str[0]);
		MetaEvent* metaEvent;

		switch (type)
		{
			/*case MetaEventType::SequenceNumber:
				break;*/
				/*case MetaEventType::Text:
					break;*/
					/*case MetaEventType::Copyright:
						break;*/
		case MetaEventType::TrackName:
			metaEvent = new TrackNameMetaEvent(deltaTime, fileStream, byteSize);
			break;
			/*case MetaEventType::InstrumentName:
				break;*/
				/*case MetaEventType::Lyric:
					break;*/
					/*case MetaEventType::Marker:
						break;*/
						/*case MetaEventType::CuePoint:
							break;*/
							/*case MetaEventType::MidiChannel:
								break;*/
		case MetaEventType::EndOfTrack:
			metaEvent = new EndTrackMetaEvent(deltaTime, fileStream, byteSize);
			break;
		case MetaEventType::Tempo:
			metaEvent = new TempoMetaEvent(deltaTime, fileStream, byteSize);
			break;
			/*case MetaEventType::SMTPE:
				break;*/
		case MetaEventType::TimeSignature:
			metaEvent = new TimeSignatureMetaEvent(deltaTime, fileStream, byteSize);
			break;
			/*case MetaEventType::KeySignature:
				break;*/
				/*case MetaEventType::SequencerSpecific:
					break;*/
		default:
			metaEvent = new MetaEvent(deltaTime, fileStream, byteSize);
			break;
		}

		if (deltaTime == 0)
			metaEvent->Apply(this);

		events.emplace_back(metaEvent);
	}
}
