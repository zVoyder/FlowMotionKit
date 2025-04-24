// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/FlowMotionFactory.h"
#include "FlowMotion.h"

UMotionMachine* UFlowMotionFactory::CreateMotionMachine(UMotionContext* InContext)
{
	UMotionMachine* NewMachine = NewObject<UMotionMachine>();
	NewMachine->Init(InContext);
	return NewMachine;
}

UMotionState* UFlowMotionFactory::CreateMotionState(UObject* Outer, const TSubclassOf<UMotionState>& InStateClass)
{
	if (!InStateClass || !IsValid(Outer))
	{
		UE_LOG(LogFlowMotion, Error, TEXT("UFlowMotionFactory::CreateMotionState: Cannot create MotionState, invalid state class or outer object."));
		return nullptr;
	}

	UMotionState* NewState = NewObject<UMotionState>(Outer, InStateClass);
	NewState->Init();
	return NewState;
}

UWallRunContext* UFlowMotionFactory::CreateWallRunContext(UWallRunner* InWallRunner, UCharacterMovementComponent* InMovementComponent)
{
	if (!InWallRunner || !IsValid(InMovementComponent))
	{
		UE_LOG(LogFlowMotion, Error, TEXT("UFlowMotionFactory::CreateWallRunContext: Cannot create WallRunContext, invalid wall runner or movement component."));
		return nullptr;
	}

	UWallRunContext* NewContext = NewObject<UWallRunContext>(InWallRunner, UWallRunContext::StaticClass());
	NewContext->Runner = InWallRunner;
	NewContext->Owner = InWallRunner->GetOwner();
	NewContext->MovementComponent = InMovementComponent;
	return NewContext;
}

URailGrindContext* UFlowMotionFactory::CreateRailGrindContext(URailGrinder* InRailGrinder, UCharacterMovementComponent* InMovementComponent)
{
	if (!InRailGrinder || !IsValid(InMovementComponent))
	{
		UE_LOG(LogFlowMotion, Error, TEXT("UFlowMotionFactory::CreateRailGrindContext: Cannot create RailGrindContext, invalid rail grinder or movement component."));
		return nullptr;
	}

	URailGrindContext* NewContext = NewObject<URailGrindContext>(InRailGrinder, URailGrindContext::StaticClass());
	NewContext->RailGrinder = InRailGrinder;
	NewContext->Owner = InRailGrinder->GetOwner();
	NewContext->MovementComponent = InMovementComponent;
	return NewContext;
}
