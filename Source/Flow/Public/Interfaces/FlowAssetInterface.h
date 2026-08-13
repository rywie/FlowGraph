#pragma once

#include "CoreMinimal.h"

#include "FlowAssetInterface.generated.h"

class UFlowNode;

/**
 * 
 */
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UFlowAssetOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOW_API IFlowAssetOwnerInterface
{
	GENERATED_BODY()

public:
	virtual UObject* GetAssetOwningObject() const = 0;
	virtual void OnNodeInstanceInitialized(UFlowNode* Node) = 0;
	virtual void OnRootFlowFinish() = 0;
};
