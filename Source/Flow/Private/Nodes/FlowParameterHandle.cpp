#include "Nodes/FlowParameterHandle.h"

/**
 *
 */
FFlowParameterHandle FFlowParameterHandle::GenerateNewHandle()
{
	static int32 GHandleID = 0;
	FFlowParameterHandle FlowParameterHandle(GHandleID = (GHandleID + 1) % INT_MAX);

	return FlowParameterHandle;
}