#pragma once

#include "Nodes/FlowNode.h"

#include "FlowNode_PathRoute.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Path Route"))
class FLOW_API UFlowNode_PathRoute final : public UFlowNode
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

	virtual TArray<FFlowPin> GetContextInputs() override;
	virtual TArray<FFlowPin> GetContextOutputs() override;

	virtual FString GetNodeDescription() const override;
	virtual FString GetStatusString() const override;
#endif
};
