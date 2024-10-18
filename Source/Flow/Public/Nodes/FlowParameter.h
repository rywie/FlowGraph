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
		: Handle(FFlowParameterHandle()), FlowPath(NAME_None)
	{
	}

	FFlowParameter(const FFlowParameterHandle& InHandle)
		: Handle(InHandle), FlowPath(NAME_None)
	{
	}

	FFlowParameter(const FFlowParameterHandle& InHandle, const FName& InFlowPath)
		: Handle(InHandle), FlowPath(InFlowPath)
	{
	}

public:
	UPROPERTY()
	FFlowParameterHandle Handle;

	UPROPERTY()
	FName FlowPath;
};
