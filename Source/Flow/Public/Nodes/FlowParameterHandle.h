#pragma once

#include "CoreMinimal.h"

#include "FlowParameterHandle.generated.h"

USTRUCT(BlueprintType)
struct FLOW_API FFlowParameterHandle
{
	GENERATED_USTRUCT_BODY()

	static FFlowParameterHandle GenerateNewHandle();

public:
	bool IsValidHandle() const
	{
		return Handle != INDEX_NONE;
	}

	bool operator==(const FFlowParameterHandle &Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FFlowParameterHandle &Other) const
	{
		return Handle != Other.Handle;
	}

	friend uint32 GetTypeHash(const FFlowParameterHandle &SpecHandle)
	{
		return ::GetTypeHash(SpecHandle.Handle);
	}

	FString ToString() const
	{
		return IsValidHandle() ? FString::FromInt(Handle) : TEXT("Invalid");
	}

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
	int32 Handle;
};