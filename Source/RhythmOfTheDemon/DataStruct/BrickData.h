#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Brick/AbstractBrick.h"
#include "BrickData.generated.h"

USTRUCT(BlueprintType)
struct FBrickDataStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
		TSubclassOf<AAbstractBrick> brickClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
		int32 TimelinePosition = 0;
};