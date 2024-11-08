#include "Nodes/Route/FlowNode_PathRoute.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_PathRoute)

/**
 * 
 */
UFlowNode_PathRoute::UFlowNode_PathRoute(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bUseExistingParameter(true)
{
#if WITH_EDITOR
	Category = TEXT("Route");
	NodeStyle = EFlowNodeStyle::Logic;
#endif
	
	InputPins = {};
	OutputPins = {};
}

void UFlowNode_PathRoute::ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter)
{
	if (Paths.Contains(PinName))
	{
		if (bUseExistingParameter)
		{
			TriggerOutput(PinName, false, EFlowPinActivationType::Default, FFlowParameter(FlowParameter.Handle, PinName));
		}
		else
		{
			TriggerOutput(PinName, false, EFlowPinActivationType::Default, FFlowParameter(FFlowParameterHandle::GenerateNewHandle(), PinName));
		}
	}
}

void UFlowNode_PathRoute::Cleanup()
{
}

#if WITH_EDITOR
TArray<FFlowPin> UFlowNode_PathRoute::GetContextInputs()
{
	TArray<FFlowPin> Pins;
	for (const auto& Path : Paths)
	{
		Pins.Emplace(FFlowPin(Path));
	}

	return Pins;
}

TArray<FFlowPin> UFlowNode_PathRoute::GetContextOutputs()
{
	TArray<FFlowPin> Pins;
	for (const auto& Path : Paths)
	{
		Pins.Emplace(FFlowPin(Path));
	}

	return Pins;
}

FString UFlowNode_PathRoute::GetNodeDescription() const
{
	TStringBuilder<32> StringBuilder;
	StringBuilder.Append(FString::Printf(TEXT("Routing Paths:")));
	StringBuilder.Append(FString::Printf(TEXT("\n")));
	
	for (const auto& Path : Paths)
	{
		StringBuilder.Append(FString::Printf(TEXT("> %s"), *Path.ToString()));
		StringBuilder.Append(FString::Printf(TEXT("\n")));
	}

	return StringBuilder.ToString();
}

FString UFlowNode_PathRoute::GetStatusString() const
{
	return Super::GetStatusString();
}
#endif
