#pragma once

#include "CoreMinimal.h"

#include "FlowNode_SubGraph_Interface.generated.h"

class UFlowNode;
class UFlowAsset;
class IFlowDataPinValueSupplierInterface;
struct FFlowParameter;
struct FFlowAssetSaveData;

UINTERFACE()
class UFlowNodeSubGraphInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOW_API IFlowNodeSubGraphInterface
{
	GENERATED_BODY()

public:
	virtual UFlowNode* GetOwningFlowNode() const = 0;
	virtual UFlowAsset* GetOwningFlowAsset() const = 0;
	virtual TSoftObjectPtr<UFlowAsset> GetSubFlowAsset() const = 0;
	virtual TScriptInterface<IFlowDataPinValueSupplierInterface> GetFlowDataPinValueSupplierInterface() const = 0;

public:
	virtual void OnFinishOutput(const FFlowParameter& FlowParameter) = 0;
	virtual void OnFlowAssetSave(const FFlowAssetSaveData& SubAssetRecord) = 0;
};
