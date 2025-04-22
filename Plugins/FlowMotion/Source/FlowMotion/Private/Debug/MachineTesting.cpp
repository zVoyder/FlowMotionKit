// Copyright VUEDK, Inc. All Rights Reserved.

#include "Debug/MachineTesting.h"
#include "Debug/TestingState.h"
#include "Factories/FlowMotionFactory.h"

UMachineTesting::UMachineTesting(): Machine(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMachineTesting::SetState1() const
{
	Machine->TryChangeState("State1");
}

void UMachineTesting::SetState2() const
{
	Machine->TryChangeState("State2");
}

void UMachineTesting::StartMachine() const
{
	Machine->StartMachine("State1");
}

void UMachineTesting::StopMachine() const
{
	Machine->StopMachine();
}

void UMachineTesting::BeginPlay()
{
	Super::BeginPlay();

	Machine = UFlowMotionFactory::CreateMotionMachine();

	UMotionState* State1 = UFlowMotionFactory::CreateMotionState(this, UTestingState::StaticClass());
	UMotionState* State2 = UFlowMotionFactory::CreateMotionState(this, UTestingState::StaticClass());

	Machine->AddState("State1", State1);
	Machine->AddState("State2", State2);
}

void UMachineTesting::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
