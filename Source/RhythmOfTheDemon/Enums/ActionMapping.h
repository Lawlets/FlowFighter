#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"


UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ EACTION_MAPPING : uint8
{
	DashVertical	UMETA(DisplayName = "Dash Vertical"),
	DashHorizontal	UMETA(DisplayName = "Dash Horizontal"),
	//Crouch			UMETA(DisplayName = "Crouch/Slide"),
	RevertA			UMETA(DisplayName = "Revert A"),
	//RevertX			UMETA(DisplayName = "Revert X"),
	RevertB			UMETA(DisplayName = "Revert B"),
	//RevertY			UMETA(DisplayName = "Revert Y"),
	CastSoundWave	UMETA(DisplayName = "SoundWave"),
	//Shoot			UMETA(DisplayName = "Shoot"),
	None			UMETA(DisplayName = "None"),
	Count
};
