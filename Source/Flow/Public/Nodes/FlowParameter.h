#pragma once

#include "CoreMinimal.h"
#include "FlowParameterHandle.h"

#include "FlowParameter.generated.h"

USTRUCT(BlueprintType)
struct FLOW_API FFlowParameter
{
	GENERATED_USTRUCT_BODY()

public:
	FFlowParameter()
		: Handle(FFlowParameterHandle())
	{
	}

	FFlowParameter(const FFlowParameterHandle &InHandle)
		: Handle(InHandle)
	{
	}

public:
	UPROPERTY()
	FFlowParameterHandle Handle;
};