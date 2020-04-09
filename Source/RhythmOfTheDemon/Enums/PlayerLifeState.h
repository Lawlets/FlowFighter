#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"


UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ EPLAYER_LIFE_STATE : uint8
{
	None			UMETA(DisplayName = "None"),
	Danger			UMETA(DisplayName = "Danger"),
	Good			UMETA(DisplayName = "Good"),
	Great			UMETA(DisplayName = "Great"),
};
