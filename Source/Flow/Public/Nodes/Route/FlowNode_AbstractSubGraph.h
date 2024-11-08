// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Nodes/FlowNode.h"
#include "FlowNode_AbstractSubGraph.generated.h"

/**
 * Creates instance of provided Flow Asset and starts its execution
 */
UCLASS(Abstract, NotBlueprintable)
class FLOW_API UFlowNode_AbstractSubGraph : public UFlowNode
{
	GENERATED_UCLASS_BODY()
	friend class UFlowAsset;
	friend class FFlowNode_SubGraphDetails;
	friend class UFlowSubsystem;

	static FFlowPin StartPin;
	static FFlowPin FinishPin;

private:
	/*
	 * Allow to create instance of the same Flow Asset as the asset containing this node
	 * Enabling it may cause an infinite loop, if graph would keep creating copies of itself
	 */
	UPROPERTY(EditAnywhere, Category = "Graph")
	bool bCanInstanceIdenticalAsset;

	UPROPERTY(SaveGame)
	FString SavedAssetInstanceName;

	UPROPERTY(SaveGame)
	FFlowParameter CachedFlowParameter;

protected:
	virtual bool CanBeAssetInstanced() const;

	virtual void PreloadContent() override;
	virtual void FlushContent() override;

	virtual void ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter) override;
	virtual void ExecuteSubGraphStartInput(const FName& Name, const FFlowParameter& FlowParameter);
	virtual void ExecuteSubGraphCustomInput(const FName& Name, const FFlowParameter& FlowParameter);
	virtual void Cleanup() override;

	virtual void CreateSubFlow(const FString SavedInstanceName, const bool bPreloading = false,
	                           const FFlowParameter& FlowParameter = FFlowParameter());

	// Call the Default Start Node Entry Input
	virtual void TriggerEntryInput(const FFlowParameter& FlowParameter = FFlowParameter());
	virtual void TriggerCustomInput(const FName& EventName, const FFlowParameter& FlowParameter = FFlowParameter());

	virtual void OnFinishOutput(const FFlowParameter& FlowParameter);

	virtual TSoftObjectPtr<UFlowAsset> GetSubAsset() const;

public:
	virtual void ForceFinishNode() override;

protected:
	virtual void OnLoad_Implementation() override;

#if WITH_EDITORONLY_DATA

protected:
	// All the classes allowed to be used as assets on this subgraph node
	UPROPERTY()
	TArray<TSubclassOf<UFlowAsset>> AllowedAssignedAssetClasses;

	// All the classes disallowed to be used as assets on this subgraph node
	UPROPERTY()
	TArray<TSubclassOf<UFlowAsset>> DeniedAssignedAssetClasses;
#endif

#if WITH_EDITOR

public:
	virtual FString GetNodeDescription() const override;
	virtual UObject* GetAssetToEdit() override;
	virtual EDataValidationResult ValidateNode() override;

	virtual bool SupportsContextPins() const override { return true; }

	virtual TArray<FFlowPin> GetContextInputs() override;
	virtual TArray<FFlowPin> GetContextOutputs() override;

	// UObject
	virtual void PostLoad() override;
	// --

protected:
	void SubscribeToAssetChanges();
#endif
};
