#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

UENUM(BlueprintType)
enum class EventType : uint8
{
	Unknown = 0xff,
	Channel = 0,
	Meta,
	SystemEx,
};

UENUM(BlueprintType)
enum class ChannelEventType : uint8
{
	Unknown = 0xff,
	NoteOff = 0x8,					// 1 : note				, 2: velocity
	NoteOn = 0x9,					// 1 : note				, 2: velocity
	NoteAftertouch = 0xa,			// 1 : note				, 2: aftertouch value
	Controller = 0xb,				// 1 : controller nb	, 2: controller value
	ProgramChange = 0xc,			// 1 : program nb
	ChannelAfterTouch = 0xd,		// 1 : aftertouch value
	PitchBend = 0xe					// 1 : pitch value (LSB), 2: pitch value (MSB)
};

UENUM(BlueprintType)
enum class MetaEventType : uint8
{
	Unknown = 0xff,
	SequenceNumber = 0x0,
	Text,				// 1
	Copyright,			// 2
	TrackName,			// 3
	InstrumentName,		// 4
	Lyric,				// 5
	Marker,				// 6
	CuePoint,			// 7
	MidiChannel = 0x20,
	EndOfTrack = 0x2f,
	Tempo = 0x51,
	SMTPE = 0x54,
	TimeSignature = 0x58,
	KeySignature = 0x59,
	SequencerSpecific = 0x7f,
};

UENUM(BlueprintType)
enum class Note : uint8
{
	None = 0xff,
	Do = 0,
	DoDiese,   // = 1
	Re,		   // = 2
	ReDiese,   // = 3
	Mi,		   // = 4
	Fa,		   // = 5
	FaDiese,   // = 6
	Sol,	   // = 7
	SolDiese,  // = 8
	La,		   // = 9
	LaDiese,   // = 10
	Si		   // = 11
};