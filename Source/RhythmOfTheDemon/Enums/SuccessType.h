#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

UENUM(BlueprintType)
enum class /*RHYTHMOFTHEDEMON_API*/ ESUCCESS_TYPE : uint8
{
	Perfect		UMETA(DisplayName = "Perfect"),
	Good		UMETA(DisplayName = "Good"),
	Ok			UMETA(DisplayName = "Ok"),
	Miss		UMETA(DisplayName = "Miss"),
};