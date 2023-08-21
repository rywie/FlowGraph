#pragma once

#include "CoreMinimal.h"
#include "FlowParameterHandle.h"

#include "FlowParameter.generated.h"

USTRUCT(BlueprintType)
struct FFlowParameter
{
	GENERATED_USTRUCT_BODY()

public:
	FFlowParameter()
		: Handle(FFlowParameterHandle())
	{
	}

	FFlowParameter(FFlowParameterHandle InHandle)
		: Handle(InHandle)
	{
	}

public:
	UPROPERTY()
	FFlowParameterHandle Handle;
};