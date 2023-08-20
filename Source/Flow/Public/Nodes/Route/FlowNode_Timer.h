// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Engine/EngineTypes.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_Timer.generated.h"

/**
 * Triggers outputs after time elapsed
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Timer", Keywords = "delay, step, tick"))
class FLOW_API UFlowNode_Timer : public UFlowNode
{
	GENERATED_UCLASS_BODY()

protected:
	// If the value is closer to 0, Timer will complete in next tick
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = 0.0f))
	float CompletionTime;

	// this allows to trigger other nodes multiple times before completing the Timer
	UPROPERTY(EditAnywhere, Category = "Timer", meta = (ClampMin = 0.0f))
	float StepTime;

private:
	FTimerHandle CompletionTimerHandle;
	FTimerHandle StepTimerHandle;

	UPROPERTY(SaveGame)
	float SumOfSteps;

	UPROPERTY(SaveGame)
	float RemainingCompletionTime;

	UPROPERTY(SaveGame)
	float RemainingStepTime;

	UPROPERTY(SaveGame)
	FFlowParameter CachedFlowParameter;

protected:
	virtual void ExecuteInput(const FName &PinName, const FFlowParameter &FlowParameter = FFlowParameter()) override;

	virtual void SetTimer(const FFlowParameter &FlowParameter = FFlowParameter());
	virtual void Restart();
	
private:
	UFUNCTION()
	void OnStep();

	UFUNCTION()
	void OnCompletion();

	void OnParameterStep(const FFlowParameter &FlowParameter);
	void OnParameterCompletion(const FFlowParameter &FlowParameter);

protected:
	virtual void Cleanup() override;

	virtual void OnSave_Implementation() override;
	virtual void OnLoad_Implementation() override;
	
#if WITH_EDITOR
	virtual FString GetNodeDescription() const override;
	virtual FString GetStatusString() const override;
#endif
};
