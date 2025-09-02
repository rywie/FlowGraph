#include "Nodes/FlowParameterHandle.h"

/**
 *
 */
FFlowParameterHandle FFlowParameterHandle::GenerateNewHandle()
{
	static int64 GHandleID = 0;
	FFlowParameterHandle FlowParameterHandle(GHandleID = (GHandleID + 1));

	return FlowParameterHandle;
}
