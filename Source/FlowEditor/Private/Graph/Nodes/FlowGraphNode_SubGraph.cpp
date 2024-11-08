// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Graph/Nodes/FlowGraphNode_SubGraph.h"
#include "Graph/Widgets/SFlowGraphNode_SubGraph.h"

#include "Nodes/Route/FlowNode_AbstractSubGraph.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowGraphNode_SubGraph)

UFlowGraphNode_SubGraph::UFlowGraphNode_SubGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AssignedNodeClasses = {UFlowNode_AbstractSubGraph::StaticClass()};
}

TSharedPtr<SGraphNode> UFlowGraphNode_SubGraph::CreateVisualWidget()
{
	return SNew(SFlowGraphNode_SubGraph, this);
}
