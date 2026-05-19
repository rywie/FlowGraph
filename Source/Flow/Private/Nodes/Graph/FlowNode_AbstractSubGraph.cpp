// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Graph/FlowNode_AbstractSubGraph.h"

#include "FlowAsset.h"
#include "FlowSettings.h"
#include "FlowSubsystem.h"
#include "Interfaces/FlowNodeWithExternalDataPinSupplierInterface.h"
#include "Types/FlowAutoDataPinsWorkingData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_AbstractSubGraph)

#define LOCTEXT_NAMESPACE "FlowNode_AbstractSubGraph"

FFlowPin UFlowNode_AbstractSubGraph::StartPin(TEXT("Start"));
FFlowPin UFlowNode_AbstractSubGraph::FinishPin(TEXT("Finish"));
const FName UFlowNode_AbstractSubGraph::AssetParams_MemberName = GET_MEMBER_NAME_CHECKED(ThisClass, AssetParams);

UFlowNode_AbstractSubGraph::UFlowNode_AbstractSubGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , bCanInstanceIdenticalAsset(false)
{
#if WITH_EDITOR
	Category = TEXT("Graph");
	NodeDisplayStyle = FlowNodeStyle::SubGraph;

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
		GetFlowSubsystem()->CreateSubFlow(this, FString(), true);
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

		Finish(FlowParameter);
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

	Super::Cleanup();
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

UFlowNode* UFlowNode_AbstractSubGraph::GetOwningFlowNode() const
{
	return const_cast<UFlowNode_AbstractSubGraph*>(this);
}

UFlowAsset* UFlowNode_AbstractSubGraph::GetOwningFlowAsset() const
{
	return GetFlowAsset();
}

TSoftObjectPtr<UFlowAsset> UFlowNode_AbstractSubGraph::GetSubFlowAsset() const
{
	return GetSubAsset();
}

void UFlowNode_AbstractSubGraph::OnFinishOutput(const FFlowParameter& FlowParameter)
{
}

TScriptInterface<IFlowDataPinValueSupplierInterface> UFlowNode_AbstractSubGraph::GetFlowDataPinValueSupplierInterface() const
{
	return const_cast<UFlowNode_AbstractSubGraph*>(this);
}

void UFlowNode_AbstractSubGraph::OnFlowAssetSave(const FFlowAssetSaveData& SubAssetRecord)
{
	SavedAssetInstanceName = SubAssetRecord.InstanceName;
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

FText UFlowNode_AbstractSubGraph::K2_GetNodeTitle_Implementation() const
{
	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (GetDefault<UFlowSettings>()->bUseAdaptiveNodeTitles && !Asset.IsNull())
	{
		return FText::Format(LOCTEXT("SubGraphTitle", "{0}\n{1}"), {Super::K2_GetNodeTitle_Implementation(), FText::FromString(Asset.ToSoftObjectPath().GetAssetName())});
	}

	return Super::K2_GetNodeTitle_Implementation();
}

FString UFlowNode_AbstractSubGraph::GetNodeDescription() const
{
	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (!GetDefault<UFlowSettings>()->bUseAdaptiveNodeTitles && !Asset.IsNull())
	{
		return Asset.ToSoftObjectPath().GetAssetName();
	}

	return Super::GetNodeDescription();;
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

TArray<FFlowPin> UFlowNode_AbstractSubGraph::GetContextInputs() const
{
	TArray<FFlowPin> ContextInputPins = Super::GetContextInputs();

	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (!Asset.IsNull())
	{
		(void)Asset.LoadSynchronous();
		if (Asset.IsValid())
		{
			for (const FName& PinName : Asset->GetCustomInputs())
			{
				if (!PinName.IsNone())
				{
					ContextInputPins.AddUnique(FFlowPin(PinName));
				}
			}
		}
	}

	return ContextInputPins;
}

TArray<FFlowPin> UFlowNode_AbstractSubGraph::GetContextOutputs() const
{
	TArray<FFlowPin> ContextOutputPins = Super::GetContextOutputs();

	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (!Asset.IsNull())
	{
		(void)Asset.LoadSynchronous();
		if (Asset.IsValid())
		{
			for (const FName& PinName : Asset->GetCustomOutputs())
			{
				if (!PinName.IsNone())
				{
					ContextOutputPins.AddUnique(FFlowPin(PinName));
				}
			}
		}
	}

	return ContextOutputPins;
}

void UFlowNode_AbstractSubGraph::AutoGenerateDataPins(FFlowAutoDataPinsWorkingData& InOutWorkingData) const
{
	Super::AutoGenerateDataPins(InOutWorkingData);

	const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset();
	if (Asset.IsNull())
	{
		return;
	}

	(void)Asset.LoadSynchronous();
	if (!Asset.IsValid())
	{
		return;
	}

	for (TPair<FGuid, TObjectPtr<UFlowNode>>& Node : Asset->Nodes)
	{
		if (const IFlowNodeWithExternalDataPinSupplierInterface* ExternalPinSuppliedNode = Cast<IFlowNodeWithExternalDataPinSupplierInterface>(Node.Value))
		{
			// If subgraph's current Flow Node uses an external data supplier (that will be this subgraph node),
			// We need to scrape the external input pins from the node and add them to our auto-generated pins list

			TArray<FFlowPin> ExternalInputPins;
			if (ExternalPinSuppliedNode->TryAppendExternalInputPins(ExternalInputPins))
			{
				const int32 NewNum = InOutWorkingData.AutoInputDataPinsNext.Num() + ExternalInputPins.Num();
				InOutWorkingData.AutoInputDataPinsNext.Reserve(NewNum);

				const FName PropertyOwnerObjectName = GetFName();

				for (const FFlowPin& FlowPin : ExternalInputPins)
				{
					InOutWorkingData.AutoInputDataPinsNext.Add(FFlowPinSourceData(FlowPin, PropertyOwnerObjectName));
				}
			}
		}
	}
}

FFlowDataPinResult UFlowNode_AbstractSubGraph::TrySupplyDataPin(FName PinName) const
{
	if (PinName == AssetParams_MemberName)
	{
		// Prevent infinite recursion by sourcing the AssetParams pin directly 
		// (otherwise, it would attempt to resolve it below and infinitely crash our stack.
		// don't ask me how I know).
		return Super::TrySupplyDataPin(PinName);
	}

	if (!IsInputConnected(PinName))
	{
		const bool bHasAssetParams = IsInputConnected(AssetParams_MemberName) || !AssetParams.IsNull();
		if (bHasAssetParams)
		{
			// If not connected, we can source the value from the asset data params (if available)
			TObjectPtr<UObject> Value = nullptr;
			const EFlowDataPinResolveResult ResultEnum = Super::TryResolveDataPinValue<FFlowPinType_Object>(AssetParams_MemberName, Value);
			if (FlowPinType::IsSuccess(ResultEnum) && IsValid(Value))
			{
				if (const IFlowDataPinValueSupplierInterface* SupplierInterface = Cast<IFlowDataPinValueSupplierInterface>(Value))
				{
					return SupplierInterface->TrySupplyDataPin(PinName);
				}
				else
				{
					LogError(FString::Printf(TEXT("Could not cast object %s to IFlowDataPinValueSupplierInterface!  This is unexpected."), *Value->GetName()));

					return FFlowDataPinResult(EFlowDataPinResolveResult::FailedWithError);
				}
			}
		}
	}

	// Prefer the standard lookup if the pin is connected 
	// (or if there is no FlowAssetParams to ask)
	return Super::TrySupplyDataPin(PinName);
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

void UFlowNode_AbstractSubGraph::UnsubscribeToAssetChanges()
{
	if (const TSoftObjectPtr<UFlowAsset> Asset = GetSubAsset())
	{
		Asset->OnSubGraphReconstructionRequested.Unbind();
	}
}
#endif

#undef LOCTEXT_NAMESPACE
