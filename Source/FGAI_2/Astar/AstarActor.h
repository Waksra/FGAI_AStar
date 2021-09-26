#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstarActor.generated.h"

class AFGGridActor;

UCLASS()
class FGAI_2_API AAstarActor : public AActor
{
	GENERATED_BODY()

public:
	AAstarActor();

	UPROPERTY()
	AFGGridActor* Grid;

protected:
	virtual void BeginPlay() override;

	virtual float Heuristic(int32 Start, int32 End);

public:
	UFUNCTION(BlueprintCallable)
	virtual TArray<int32> FindPath(int32 StartIndex, int32 GoalIndex);
};
