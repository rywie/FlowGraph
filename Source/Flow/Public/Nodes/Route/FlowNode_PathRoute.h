#pragma once

#include "FlowNodeBase_RouteNode.h"

#include "FlowNode_PathRoute.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Path Route"))
class FLOW_API UFlowNode_PathRoute final : public UFlowNodeBase_RouteNode
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter = FFlowParameter()) override;
	virtual void Cleanup() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FName> Paths;

	UPROPERTY(EditAnywhere)
	bool bUseExistingParameter;

#if WITH_EDITOR

public:
	virtual bool SupportsContextPins() const override { return true; }

	virtual TArray<FFlowPin> GetContextInputs() const override;
	virtual TArray<FFlowPin> GetContextOutputs() const override;

	virtual FString GetNodeDescription() const override;
	virtual FString GetStatusString() const override;
#endif
};
