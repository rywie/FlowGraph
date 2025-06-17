// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "FlowNodeBase_RouteNode.h"
#include "FlowNode_Reroute.generated.h"

/**
 * Reroute
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Reroute"))
class FLOW_API UFlowNode_Reroute final : public UFlowNodeBase_RouteNode
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void ExecuteInput(const FName &PinName, const FFlowParameter &FlowParameter = FFlowParameter()) override;
};
