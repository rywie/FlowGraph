#pragma once

#include "CoreMinimal.h"

#include "FlowParameterHandle.generated.h"

USTRUCT(BlueprintType)
struct FFlowParameterHandle
{
	GENERATED_USTRUCT_BODY()

public:
	static FFlowParameterHandle GenerateNewHandle();

	bool IsValidHandle() const;

public:
	FFlowParameterHandle()
		: Handle(INDEX_NONE)
	{
	}
	FFlowParameterHandle(int32 InHandle)
		: Handle(InHandle)
	{
	}

public:
	UPROPERTY()
	int32 Handle;
};