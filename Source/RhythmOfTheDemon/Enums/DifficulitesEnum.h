#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"


UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ EDIFFICULTIES : uint8
{
	Easy			UMETA(DisplayName = "Easy"),
	Normal			UMETA(DisplayName = "Normal"),
	Hard			UMETA(DisplayName = "Hard"),
};
