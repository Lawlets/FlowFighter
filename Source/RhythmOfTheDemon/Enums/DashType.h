#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ EDASH_TYPE : uint8
{
	HorizontalDash	UMETA(DisplayName = "Horizontal Dash"),
	VerticalDash	UMETA(DisplayName = "Vertical Dash"),
	None			UMETA(DisplayName = "None"),
	Count,
};
