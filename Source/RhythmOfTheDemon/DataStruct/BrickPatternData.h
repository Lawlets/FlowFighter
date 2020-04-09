#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BrickData.h"
#include "BrickPatternData.generated.h"

USTRUCT(BlueprintType)
struct FBrickPatternDataStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
		TArray<FBrickDataStruct> BrickArray;

};