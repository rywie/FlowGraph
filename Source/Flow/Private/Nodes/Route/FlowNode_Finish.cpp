// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Route/FlowNode_Finish.h"

#include "FlowAsset.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_Finish)

UFlowNode_Finish::UFlowNode_Finish(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bTriggerFinish(true)
{
#if WITH_EDITOR
	Category = TEXT("Route");
	NodeStyle = EFlowNodeStyle::InOut;
#endif

	OutputPins = {};
	AllowedSignalModes = {EFlowSignalMode::Enabled, EFlowSignalMode::Disabled};
}

void UFlowNode_Finish::ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter /*= FFlowParameter()*/)
{
	const UFlowAsset* FlowAsset = GetFlowAsset();
	check(IsValid(FlowAsset));

	if (bTriggerFinish)
	{
		// this will call FinishFlow()
		Finish(FlowParameter);
	}
	else
	{
		// this will tell the Flow Asset to emit the Finish Output through the owner Subgraph
		TriggerFinishOutput(FlowParameter);
	}
}
