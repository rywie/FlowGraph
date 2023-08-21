#include "Nodes/FlowParameterHandle.h"

/**
 *
 */
FFlowParameterHandle FFlowParameterHandle::GenerateNewHandle()
{
	static int32 GHandleID = 0;
	FFlowParameterHandle FlowParameterHandle(GHandleID++);

	return FlowParameterHandle;
}

bool FFlowParameterHandle::IsValidHandle() const
{
	return Handle != INDEX_NONE;
}