#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ EPLAYER_TYPE : uint8
{
	Arm			UMETA(DisplayName = "Arm"),
	Leg			UMETA(DisplayName = "Leg"),
	Both		UMETA(DisplayName = "Both"),
};