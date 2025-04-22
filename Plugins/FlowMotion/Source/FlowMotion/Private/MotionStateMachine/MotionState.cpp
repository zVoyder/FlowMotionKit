// Copyright VUEDK, Inc. All Rights Reserved.

#include "MotionStateMachine/MotionState.h"
#include "FlowMotion.h"
#include "MotionStateMachine/MotionMachine.h"

void UMotionState::Init()
{
	OnInit();
}

void UMotionState::OnInit()
{
}

void UMotionState::OnAdded(UMotionMachine* InMachine)
{
}

void UMotionState::OnRemoved(UMotionMachine* InMachine)
{
}

void UMotionState::OnEnter()
{
	UE_LOG(LogFlowMotion, Display, TEXT("UMotionState::OnEnter: State %s entered."), *GetName());
}

void UMotionState::OnProcess(float DeltaTime)
{
}

void UMotionState::OnExit()
{
	UE_LOG(LogFlowMotion, Display, TEXT("UMotionState::OnExit: State %s exited."), *GetName());
}

void UMotionState::OnAbort()
{
}

void UMotionState::TransitionTo(const FName& StateName) const
{
	Machine->TryChangeState(StateName);
}

UMotionMachine* UMotionState::GetMachine() const
{
	return Machine;
}

UMotionContext* UMotionState::GetMachineContext() const
{
	if (!IsValid(Machine))
		return nullptr;

	return Machine->GetContext();
}

void UMotionState::AddInMachine(UMotionMachine* InMachine)
{
	Machine = InMachine;
	OnAdded(Machine);
}

void UMotionState::RemoveFromMachine(UMotionMachine* InMachine)
{
	Machine = nullptr;
	OnRemoved(InMachine);
}
