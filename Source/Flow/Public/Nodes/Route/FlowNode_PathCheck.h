#pragma once

#include "Nodes/FlowNode.h"

#include "FlowNode_PathCheck.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Path Check"))
class FLOW_API UFlowNode_PathCheck final : public UFlowNode
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void ExecuteInput(const FName& PinName, const FFlowParameter& FlowParameter = FFlowParameter()) override;
	virtual void Cleanup() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FName> Paths;

#if WITH_EDITOR

public:
	virtual bool SupportsContextPins() const override { return true; }

	virtual TArray<FFlowPin> GetContextInputs() override;
	virtual TArray<FFlowPin> GetContextOutputs() override;

	virtual FString GetNodeDescription() const override;
	virtual FString GetStatusString() const override;
#endif
};
