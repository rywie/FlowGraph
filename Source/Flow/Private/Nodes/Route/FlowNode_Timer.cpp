// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Route/FlowNode_Timer.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_Timer)

UFlowNode_Timer::UFlowNode_Timer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CompletionTime(1.0f)
	, StepTime(0.0f)
	, SumOfSteps(0.0f)
	, RemainingCompletionTime(0.0f)
	, RemainingStepTime(0.0f)
{
#if WITH_EDITOR
	Category = TEXT("Route");
	NodeStyle = EFlowNodeStyle::Latent;
#endif
	bFinishFlow = true;

	InputPins.Add(FFlowPin(TEXT("Skip")));
	InputPins.Add(FFlowPin(TEXT("Restart")));

	OutputPins.Empty();
	OutputPins.Add(FFlowPin(TEXT("Completed")));
	OutputPins.Add(FFlowPin(TEXT("Step")));
	OutputPins.Add(FFlowPin(TEXT("Skipped")));
}

void UFlowNode_Timer::ExecuteInput(const FName &PinName, const FFlowParameter &FlowParameter /*= FFlowParameter()*/)
{
	if (PinName == TEXT("In"))
	{
		if (CompletionTimerHandle.IsValid() || StepTimerHandle.IsValid())
		{
			LogError(TEXT("Timer already active"));
			return;
		}

		SetTimer(FlowParameter);
	}
	else if (PinName == TEXT("Skip"))
	{
		TriggerOutput(TEXT("Skipped"), bFinishFlow, FlowParameter);
	}
	else if (PinName == TEXT("Restart"))
	{
		Restart();
	}
}

void UFlowNode_Timer::SetTimer(const FFlowParameter &FlowParameter /*= FFlowParameter()*/)
{
	if (GetWorld())
	{
		CachedFlowParameter = FlowParameter;

		if (StepTime > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, this, &UFlowNode_Timer::OnStep, StepTime, true);
		}

		if (CompletionTime > UE_KINDA_SMALL_NUMBER)
		{
			GetWorld()->GetTimerManager().SetTimer(CompletionTimerHandle, this, &UFlowNode_Timer::OnCompletion, CompletionTime, false);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFlowNode_Timer::OnCompletion);
		}
	}
	else
	{
		LogError(TEXT("No valid world"));
		TriggerOutput(TEXT("Completed"), bFinishFlow, FlowParameter);
	}
}

void UFlowNode_Timer::Restart()
{
	Cleanup();

	RemainingStepTime = 0.0f;
	RemainingCompletionTime = 0.0f;

	SetTimer();
}

void UFlowNode_Timer::OnStep()
{
	OnParameterStep(CachedFlowParameter);
}

void UFlowNode_Timer::OnParameterStep(const FFlowParameter &FlowParameter)
{
	SumOfSteps += StepTime;

	if (SumOfSteps >= CompletionTime)
	{
		TriggerOutput(TEXT("Completed"), bFinishFlow, FlowParameter);
	}
	else
	{
		TriggerOutput(TEXT("Step"), false, FlowParameter);
	}
}

void UFlowNode_Timer::OnCompletion()
{
	OnParameterCompletion(CachedFlowParameter);
}

void UFlowNode_Timer::OnParameterCompletion(const FFlowParameter &FlowParameter)
{
	TriggerOutput(TEXT("Completed"), bFinishFlow, FlowParameter);
}

void UFlowNode_Timer::Cleanup()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CompletionTimerHandle);
	}

	CompletionTimerHandle.Invalidate();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(StepTimerHandle);
	}

	StepTimerHandle.Invalidate();

	SumOfSteps = 0.0f;

	CachedFlowParameter = FFlowParameter();
}

void UFlowNode_Timer::OnSave_Implementation()
{
	if (GetWorld())
	{
		if (CompletionTimerHandle.IsValid())
		{
			RemainingCompletionTime = GetWorld()->GetTimerManager().GetTimerRemaining(CompletionTimerHandle);
		}

		if (StepTimerHandle.IsValid())
		{
			RemainingStepTime = GetWorld()->GetTimerManager().GetTimerRemaining(StepTimerHandle);
		}
	}
}

void UFlowNode_Timer::OnLoad_Implementation()
{
	if (RemainingStepTime > 0.0f || RemainingCompletionTime > 0.0f)
	{
		if (RemainingStepTime > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, this, &UFlowNode_Timer::OnStep, StepTime, true, RemainingStepTime);
		}

		GetWorld()->GetTimerManager().SetTimer(CompletionTimerHandle, this, &UFlowNode_Timer::OnCompletion, RemainingCompletionTime, false);

		RemainingStepTime = 0.0f;
		RemainingCompletionTime = 0.0f;
	}
}

#if WITH_EDITOR
FString UFlowNode_Timer::GetNodeDescription() const
{
	if (CompletionTime > UE_KINDA_SMALL_NUMBER)
	{
		if (StepTime > 0.0f)
		{
			return FString::Printf(TEXT("%.*f, step by %.*f"), 2, CompletionTime, 2, StepTime);
		}

		return FString::Printf(TEXT("%.*f"), 2, CompletionTime);
	}

	return TEXT("Completes in next tick");
}

FString UFlowNode_Timer::GetStatusString() const
{
	if (StepTime > 0.0f)
	{
		return FString::Printf(TEXT("Progress: %.*f"), 2, SumOfSteps);
	}

	if (CompletionTimerHandle.IsValid() && GetWorld())
	{
		return FString::Printf(TEXT("Progress: %.*f"), 2, GetWorld()->GetTimerManager().GetTimerElapsed(CompletionTimerHandle));
	}

	return FString();
}
#endif
