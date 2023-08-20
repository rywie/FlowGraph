#pragma once

#include "CoreMinimal.h"

#include "FlowParameter.generated.h"

USTRUCT(BlueprintType)
struct FFlowParameter
{
	GENERATED_USTRUCT_BODY()

public:
	FFlowParameter()
		: Handle(-1)
	{
	}

	FFlowParameter(int32 InHandle)
		: Handle(InHandle)
	{
	}

public:
	UPROPERTY()
	int32 Handle;
};