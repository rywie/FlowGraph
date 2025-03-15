// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Interfaces/FlowDataPinGeneratorNodeInterface.h"
#include "FlowNode_AbstractSubGraph.h"

#include "FlowNode_SubGraph.generated.h"

UCLASS(NotBlueprintable, meta = (DisplayName = "Sub Graph"))
class FLOW_API UFlowNode_SubGraph : public UFlowNode_AbstractSubGraph
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void ExecuteSubGraphStartInput(const FName& PinName, const FFlowParameter& FlowParameter) override;
	virtual void ExecuteSubGraphCustomInput(const FName& PinName, const FFlowParameter& FlowParameter) override;

	virtual void OnFinishOutput(const FFlowParameter& FlowParameter) override;

	virtual TSoftObjectPtr<UFlowAsset> GetSubAsset() const override;

private:
	UPROPERTY(EditAnywhere, Category = "Graph")
	TSoftObjectPtr<UFlowAsset> Asset;

#if WITH_EDITOR

public:
	// UObject
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// --
#endif
};
