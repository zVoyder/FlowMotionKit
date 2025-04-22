// Copyright VUEDK, Inc. All Rights Reserved.

#include "MotionStateMachine/MotionMachine.h"
#include "FlowMotion.h"
#include "WallRunSystem/WallRunner.h"

void UMotionMachine::Init(UMotionContext* InContext)
{
	Context = InContext;
}

void UMotionMachine::StartMachine(const FName InitialStateTag)
{
	if (bIsRunning)
		return;

	if (!States.Contains(InitialStateTag))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("UMotionMachine::StartMachine: Failed to start motion machine. State %s not found."), *InitialStateTag.ToString());
		return;
	}

	bIsRunning = true;
	CurrentState = States[InitialStateTag];
	States[InitStateName]->OnEnter();
}

void UMotionMachine::StopMachine()
{
	if (!bIsRunning)
		return;

	if (IsValid(CurrentState))
		CurrentState->OnAbort();
	
	CurrentState = nullptr;
	bIsRunning = false;
}

UMotionContext* UMotionMachine::GetContext() const
{
	if (!IsValid(Context))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("UMotionMachine::GetContext: Trying to get context, but context was not set."));
		return nullptr;
	}
	
	return Context;
}

bool UMotionMachine::IsRunning() const
{
	return bIsRunning;
}

bool UMotionMachine::TryChangeState(const FName StateTag)
{
	if (!bIsRunning || !States.Contains(StateTag))
		return false;

	UMotionState* NewState = States[StateTag];

	if (NewState == CurrentState)
		return false;

	if (IsValid(CurrentState))
		CurrentState->OnExit();

	NewState->OnEnter();
	CurrentState = NewState;

	return true;
}

void UMotionMachine::AddState(const FName StateTag, UMotionState* State)
{
	if (!IsValid(State))
		return;

	State->AddInMachine(this);
	States.Add(StateTag, State);
}

UMotionState* UMotionMachine::RemoveState(const FName StateTag)
{
	if (!States.Contains(StateTag))
		return nullptr;

	UMotionState* State = States[StateTag];
	State->RemoveFromMachine(this);
	States.Remove(StateTag);
	return State;
}

bool UMotionMachine::IsStateActive(const FName StateTag) const
{
	if (!bIsRunning || !IsValid(CurrentState) || !States.Contains(StateTag))
		return false;
	
	return States[StateTag] == CurrentState;
}

void UMotionMachine::Tick(float DeltaTime)
{
	ProcessCurrentState(DeltaTime);
}

bool UMotionMachine::IsTickable() const
{
	return !States.IsEmpty() && bIsRunning;
}

TStatId UMotionMachine::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMotionMachine, STATGROUP_Tickables);
}

void UMotionMachine::ProcessCurrentState(const float DeltaTime) const
{
	if (!IsValid(CurrentState))
		return;
	
	CurrentState->OnProcess(DeltaTime);
}
