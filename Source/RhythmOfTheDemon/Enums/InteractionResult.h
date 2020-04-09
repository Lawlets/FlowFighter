#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"


UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ EINTERACTION_RESULT : uint8
{
	Valid		UMETA(DisplayName = "Valid Interaction"),
	Miss		UMETA(DisplayName = "Missed Interaction"),
	None		UMETA(DisplayName = "None"),
	Count
};