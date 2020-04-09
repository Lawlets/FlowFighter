#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ EBRICK_STATE : uint8
{
	IDLE			UMETA(DisplayName = "Idle"),
	PREPARATION		UMETA(DisplayName = "Preparation"),
	HIT				UMETA(DisplayName = "Hit"),
	PASSED			UMETA(DisplayName = "Passed"),
};