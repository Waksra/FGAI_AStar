#include "AstarActor.h"

#include "PriorityQueue.h"
#include "FGAI_2/Grid/FGGridActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAstarActor::AAstarActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAstarActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> AllGridActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGGridActor::StaticClass(), AllGridActors);

	if (AllGridActors.Num() > 0)
	{
		Grid = Cast<AFGGridActor>(AllGridActors[0]);
	}
}

float AAstarActor::Heuristic(int32 a, int32 b)
{
	return UKismetMathLibrary::Abs(a%Grid->Width - b%Grid->Width) + UKismetMathLibrary::Abs(a/Grid->Width - b/Grid->Width);
}

TArray<int32> AAstarActor::FindPath(int32 StartIndex, int32 GoalIndex)
{
	TMap<int32, int32> ParentMap;
	TMap<int32, float> CostMap;
	TPriorityQueue<int32> Frontier;

	ParentMap.Add(StartIndex, StartIndex);
	CostMap.Add(StartIndex, 0);
	Frontier.Push(StartIndex, 0);

	while (!Frontier.IsEmpty())
	{
		const int32 Current = Frontier.Pop();

		if(Current == GoalIndex)
			break;

		for (const int32 Next : Grid->GetCardinalNeighbours(Current))
		{
			const float NewCost = CostMap[Current] + 1;
			if(!CostMap.Contains(Next))
			{
				CostMap.Add(Next, NewCost);
				const float Priority = NewCost + Heuristic(Next, GoalIndex);
				Frontier.Push(Next, Priority);
				ParentMap.Add(Next, Current);
			}
			else if (NewCost < CostMap[Next])
			{
				CostMap[Next] = NewCost;
				const float Priority = NewCost + Heuristic(Next, GoalIndex);
				Frontier.Push(Next, Priority);
				ParentMap[Next] = Current;
			}
		}
	}

	TArray<int32> ReturnPath;
	
	if(!CostMap.Contains(GoalIndex))
	{
		return ReturnPath;
	}

	ReturnPath.Add(GoalIndex);
	int32 Current;
	while ((Current = ReturnPath.Top()) != StartIndex)
	{
		ReturnPath.Add(ParentMap[Current]);
	}

	Algo::Reverse(ReturnPath);
	return ReturnPath;
}
