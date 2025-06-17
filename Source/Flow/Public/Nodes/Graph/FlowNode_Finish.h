// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "FlowNodeBase_GraphNode.h"
#include "FlowNode_Finish.generated.h"

/**
 * Finish execution of this Flow Asset
 * All active nodes and sub graphs will be deactivated
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Finish"))
class FLOW_API UFlowNode_Finish : public UFlowNodeBase_GraphNode
{
	GENERATED_UCLASS_BODY()

protected:
	virtual bool CanFinishGraph() const override { return true; }
	virtual void ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter = FFlowParameter()) override;

protected:
	UPROPERTY(EditAnywhere)
	bool bTriggerFinish;
};
