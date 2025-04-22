// Copyright VUEDK, Inc. All Rights Reserved.

#include "Debug/TestingState.h"
#include "FlowMotion.h"

void UTestingState::OnEnter()
{
	Super::OnEnter();
	
	UE_LOG(LogFlowMotion, Warning, TEXT("TestingState::OnEnter() - %s"), *GetFName().ToString());
}

void UTestingState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, TEXT("TestingState::OnProcess() - ") + GetFName().ToString());
}

void UTestingState::OnExit()
{
	Super::OnExit();
	
	UE_LOG(LogFlowMotion, Warning, TEXT("TestingState::OnExit() - %s"), *GetFName().ToString());
}
