#pragma once

#include "FlowOwnerInterface.h"

#include "FlowAssetInterface.generated.h"

class UFlowNode;

/**
 * 
 */
UINTERFACE()
class UFlowAssetOwnerInterface : public UFlowOwnerInterface
{
	GENERATED_BODY()
};

class FLOW_API IFlowAssetOwnerInterface : public IFlowOwnerInterface
{
	GENERATED_BODY()

public:
	virtual UObject* GetAssetOwningObject() const = 0;
	virtual void OnNodeInstanceInitialized(UFlowNode* Node) = 0;
	virtual void OnRootFlowFinish() = 0;
};
