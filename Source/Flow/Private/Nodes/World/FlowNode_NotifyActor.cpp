// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/World/FlowNode_NotifyActor.h"
#include "FlowComponent.h"
#include "FlowSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_NotifyActor)

UFlowNode_NotifyActor::UFlowNode_NotifyActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MatchType(EGameplayContainerMatchType::All)
	, bExactMatch(true)
	, NetMode(EFlowNetMode::Authority)
{
#if WITH_EDITOR
	Category = TEXT("Notifies");
#endif
}

void UFlowNode_NotifyActor::ExecuteInput(const FName &PinName, const FFlowParameter &FlowParameter /*= FFlowParameter()*/)
{
	if (const UFlowSubsystem* FlowSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UFlowSubsystem>())
	{
		for (const TWeakObjectPtr<UFlowComponent>& Component : FlowSubsystem->GetComponents<UFlowComponent>(IdentityTags, MatchType, bExactMatch))
		{
			Component->NotifyFromGraph(NotifyTags, NetMode);
		}
	}

	TriggerFirstOutput(true, FlowParameter);
}

#if WITH_EDITOR
FString UFlowNode_NotifyActor::GetNodeDescription() const
{
	return GetIdentityTagsDescription(IdentityTags) + LINE_TERMINATOR + GetNotifyTagsDescription(NotifyTags);
}

EDataValidationResult UFlowNode_NotifyActor::ValidateNode()
{
	if (IdentityTags.IsEmpty())
	{
		ValidationLog.Error<UFlowNode>(*UFlowNode::MissingIdentityTag, this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif
