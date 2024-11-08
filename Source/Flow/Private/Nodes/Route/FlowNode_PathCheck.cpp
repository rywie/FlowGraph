#include "Nodes/Route/FlowNode_PathCheck.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_PathCheck)

/**
 * 
 */
UFlowNode_PathCheck::UFlowNode_PathCheck(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Route");
	NodeStyle = EFlowNodeStyle::Logic;
#endif
}

void UFlowNode_PathCheck::ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter)
{
	if (PinName == DefaultInputPin.PinName)
	{
		const FName OutputPinName = FlowParameter.FlowPath;
		if (Paths.Contains(OutputPinName))
		{
			TriggerOutput(OutputPinName, false, EFlowPinActivationType::Default, FlowParameter);
		}
		else
		{
			TriggerOutput(DefaultOutputPin.PinName, false, EFlowPinActivationType::Default, FlowParameter);
		}
	}
}

void UFlowNode_PathCheck::Cleanup()
{
}

#if WITH_EDITOR
TArray<FFlowPin> UFlowNode_PathCheck::GetContextInputs()
{
	TArray<FFlowPin> Pins;
	return Pins;
}

TArray<FFlowPin> UFlowNode_PathCheck::GetContextOutputs()
{
	TArray<FFlowPin> Pins;
	for (const auto& Path : Paths)
	{
		Pins.Emplace(FFlowPin(Path));
	}

	return Pins;
}

FString UFlowNode_PathCheck::GetNodeDescription() const
{
	TStringBuilder<32> StringBuilder;
	StringBuilder.Append(FString::Printf(TEXT("Output Paths:")));
	StringBuilder.Append(FString::Printf(TEXT("\n")));
	
	for (const auto& Path : Paths)
	{
		StringBuilder.Append(FString::Printf(TEXT("> %s"), *Path.ToString()));
		StringBuilder.Append(FString::Printf(TEXT("\n")));
	}

	return StringBuilder.ToString();
}

FString UFlowNode_PathCheck::GetStatusString() const
{
	return Super::GetStatusString();
}
#endif
