#include "Nodes/Route/FlowNode_SubGraph.h"

#include "FlowAsset.h"
#include "FlowSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_SubGraph)

UFlowNode_SubGraph::UFlowNode_SubGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UFlowNode_SubGraph::ExecuteSubGraphStartInput(const FName& PinName, const FFlowParameter& FlowParameter)
{
	CreateSubFlow(FString(), false, FlowParameter);
}

void UFlowNode_SubGraph::ExecuteSubGraphCustomInput(const FName& PinName, const FFlowParameter& FlowParameter)
{
	TriggerCustomInput(PinName, FlowParameter);
}

void UFlowNode_SubGraph::OnFinishOutput(const FFlowParameter& FlowParameter)
{
	TriggerFirstOutput(false, FlowParameter);
}

TSoftObjectPtr<UFlowAsset> UFlowNode_SubGraph::GetSubAsset() const
{
	return Asset;
}

#if WITH_EDITOR
void UFlowNode_SubGraph::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);

	if (PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(UFlowNode_SubGraph, Asset))
	{
		if (Asset)
		{
			Asset->OnSubGraphReconstructionRequested.Unbind();
		}
	}
}

void UFlowNode_SubGraph::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UFlowNode_SubGraph, Asset))
	{
		OnReconstructionRequested.ExecuteIfBound();
		SubscribeToAssetChanges();
	}
}
#endif
