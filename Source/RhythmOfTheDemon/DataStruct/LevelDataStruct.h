#pragma once

#include "CoreMinimal.h"
#include "FMODEvent.h"
#include "Engine/DataTable.h"
#include "Curves/CurveFloat.h"

#include "Actors/AbstractEntityActor.h"

#include "LevelDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FLevelDataStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FString name = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		int32 levelIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		bool isTutorial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		UFMODEvent* soundEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FString midiFileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		TSubclassOf <AAbstractEntityActor> enemyActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		UCurveFloat* speedCurve;

};