#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ ECAMERA_VIEW : uint8
{
	PAUSE				UMETA(DisplayName = "PAUSE"),
	END_LEVEL			UMETA(DisplayName = "END_LEVEL"),
	QUARTER_RIGHT		UMETA(DisplayName = "QUARTER_RIGHT"),
	QUARTER_LEFT		UMETA(DisplayName = "QUARTER_LEFT"),
	MENU_IDLE			UMETA(DisplayName = "MENU_IDDLE"),
	END_LEVEL1_MATINEE	UMETA(DisplayName = "END_LEVEL1_MATINEE"),
	END_LEVEL2_MATINEE	UMETA(DisplayName = "END_LEVEL2_MATINEE"),
	END_LEVEL3_MATINEE	UMETA(DisplayName = "END_LEVEL3_MATINEE")
};