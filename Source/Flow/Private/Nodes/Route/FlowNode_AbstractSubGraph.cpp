// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Route/FlowNode_AbstractSubGraph.h"

#include "FlowAsset.h"
#include "FlowMessageLog.h"
#include "FlowSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_AbstractSubGraph)

FFlowPin UFlowNode_AbstractSubGraph::StartPin(TEXT("Start"));
FFlowPin UFlowNode_AbstractSubGraph::FinishPin(TEXT("Finish"));

UFlowNode_AbstractSubGraph::UFlowNode_AbstractSubGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , bCanInstanceIdenticalAsset(false)
{
#if WITH_EDITOR
	Category = TEXT("Route");
	NodeStyle = EFlowNodeStyle::SubGraph;

	AllowedAssignedAssetClasses = {UFlowAsset::StaticClass()};
#endif

	InputPins = {StartPin};
	OutputPins = {FinishPin};
}

bool UFlowNode_AbstractSubGraph::CanBeAssetInstanced() const
{
	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	return !Asset.IsNull() && (bCanInstanceIdenticalAsset || Asset.ToString() != GetFlowAsset()->GetTemplateAsset()->GetPathName());
}

void UFlowNode_AbstractSubGraph::PreloadContent()
{
	if (CanBeAssetInstanced() && GetFlowSubsystem())
	{
		GetFlowSubsystem()->CreateSubFlow(this, FString(), true, CachedFlowParameter);
	}
}

void UFlowNode_AbstractSubGraph::FlushContent()
{
	if (CanBeAssetInstanced() && GetFlowSubsystem())
	{
		GetFlowSubsystem()->RemoveSubFlow(this, EFlowFinishPolicy::Abort);
	}
}

void UFlowNode_AbstractSubGraph::ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter)
{
	if (CanBeAssetInstanced() == false)
	{
		const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
		if (Asset.IsNull())
		{
			LogError(TEXT("Missing Flow Asset"));
		}
		else
		{
			LogError(FString::Printf(TEXT("Asset %s cannot be instance, probably is the same as the asset owning this SubGraph node."), *Asset.ToString()));
		}

		Finish();
		return;
	}

	CachedFlowParameter = FlowParameter;

	if (PinName == StartPin.PinName)
	{
		ExecuteSubGraphStartInput(PinName, FlowParameter);
	}
	else if (!PinName.IsNone())
	{
		ExecuteSubGraphCustomInput(PinName, FlowParameter);
	}
}

void UFlowNode_AbstractSubGraph::ExecuteSubGraphStartInput(const FName& Name, const FFlowParameter& FlowParameter)
{
}

void UFlowNode_AbstractSubGraph::ExecuteSubGraphCustomInput(const FName& Name, const FFlowParameter& FlowParameter)
{
}

void UFlowNode_AbstractSubGraph::Cleanup()
{
	if (CanBeAssetInstanced() && GetFlowSubsystem())
	{
		GetFlowSubsystem()->RemoveSubFlow(this, EFlowFinishPolicy::Keep);
	}
}

void UFlowNode_AbstractSubGraph::CreateSubFlow(const FString SavedInstanceName, const bool bPreloading, const FFlowParameter& FlowParameter)
{
	if (GetFlowSubsystem())
	{
		GetFlowSubsystem()->CreateSubFlow(this, SavedInstanceName, bPreloading, FlowParameter);
	}
}

void UFlowNode_AbstractSubGraph::TriggerEntryInput(const FFlowParameter& FlowParameter)
{
	GetFlowAsset()->TriggerEntryInput(this, FlowParameter);
}

void UFlowNode_AbstractSubGraph::TriggerCustomInput(const FName& EventName, const FFlowParameter& FlowParameter)
{
	if (GetFlowAsset())
	{
		GetFlowAsset()->TriggerCustomInput_FromSubGraph(this, EventName, FlowParameter);
	}
}

void UFlowNode_AbstractSubGraph::OnFinishOutput(const FFlowParameter& FlowParameter)
{
}

TSoftObjectPtr<UFlowAsset> UFlowNode_AbstractSubGraph::GetSubAsset() const
{
	return nullptr;
}

void UFlowNode_AbstractSubGraph::ForceFinishNode()
{
	TriggerFirstOutput(true, CachedFlowParameter);
}

void UFlowNode_AbstractSubGraph::OnLoad_Implementation()
{
	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (!SavedAssetInstanceName.IsEmpty() && !Asset.IsNull())
	{
		GetFlowSubsystem()->LoadSubFlow(this, SavedAssetInstanceName);
		SavedAssetInstanceName = FString();
	}
}

#if WITH_EDITOR
FString UFlowNode_AbstractSubGraph::GetNodeDescription() const
{
	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	return Asset.IsNull() ? FString() : Asset.ToSoftObjectPath().GetAssetName();
}

UObject* UFlowNode_AbstractSubGraph::GetAssetToEdit()
{
	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	return Asset.IsNull() ? nullptr : Asset.LoadSynchronous();
}

EDataValidationResult UFlowNode_AbstractSubGraph::ValidateNode()
{
	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (Asset.IsNull())
	{
		ValidationLog.Error<UFlowNode>(TEXT("Flow Asset not assigned or invalid!"), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}

TArray<FFlowPin> UFlowNode_AbstractSubGraph::GetContextInputs()
{
	TArray<FFlowPin> EventNames;

	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (!Asset.IsNull())
	{
		Asset.LoadSynchronous();
		for (const FName& PinName : Asset.Get()->GetCustomInputs())
		{
			if (!PinName.IsNone())
			{
				EventNames.Emplace(PinName);
			}
		}
	}

	return EventNames;
}

TArray<FFlowPin> UFlowNode_AbstractSubGraph::GetContextOutputs()
{
	TArray<FFlowPin> Pins;

	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (!Asset.IsNull())
	{
		Asset.LoadSynchronous();
		for (const FName& PinName : Asset.Get()->GetCustomOutputs())
		{
			if (!PinName.IsNone())
			{
				Pins.Emplace(PinName);
			}
		}
	}

	return Pins;
}

void UFlowNode_AbstractSubGraph::PostLoad()
{
	Super::PostLoad();

	SubscribeToAssetChanges();
}

void UFlowNode_AbstractSubGraph::SubscribeToAssetChanges()
{
	if (const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset())
	{
		TWeakObjectPtr<UFlowNode_AbstractSubGraph> SelfWeakPtr(this);
		Asset->OnSubGraphReconstructionRequested.BindLambda([SelfWeakPtr]()
		{
			if (SelfWeakPtr.IsValid())
			{
				SelfWeakPtr->OnReconstructionRequested.ExecuteIfBound();
			}
		});
	}
}
#endif
