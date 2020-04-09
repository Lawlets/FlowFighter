#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "RailHandler.generated.h"

UENUM(BlueprintType)
enum class RAIL_TYPES : uint8
{
	RT_LEFT			UMETA(DisplayName = "RAIL_LEFT"),
	RT_CENTER		UMETA(DisplayName = "RAIL_CENTER"),
	RT_RIGHT		UMETA(DisplayName = "RAIL_RIGHT"),
};

class ARail;
class ARhythmHandler;
class ASplineLevel;

UCLASS()
class RHYTHMOFTHEDEMON_API ARailHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	ARailHandler();
	virtual void Tick(float DeltaTime) override;

	static ARailHandler* GetInstance() { return m_instance; }

protected:
	virtual void BeginPlay() override;
	static ARailHandler* m_instance;

public:	

	//void SetupSplineCurve(FSplineCurves spline_curve);

	ARail*		GetPlayerRail() const;
	ARail*		GetPlayerRail(int32 levelIndex) const;
	ARail*		GetEnemyRail() const;
	ARail*		GetEnemyRail(int32 levelIndex) const;

	void		SetCurrentRailIndex(int32 railIndex);
	void		ClearCurrentRailIndex();

	UPROPERTY(EditAnywhere)
		TArray<ARail*> m_playerRailArray;

	UPROPERTY(EditAnywhere)
		TArray<ARail*> m_enemyRailArray;

private:

	ARhythmHandler* m_rhythmHandler;
	int m_railIndex;
};
