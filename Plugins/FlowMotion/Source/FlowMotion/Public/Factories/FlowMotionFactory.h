// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MotionStateMachine/MotionMachine.h"
#include "RailGrinderSystem/Machine/RailGrindContext.h"
#include "WallRunSystem/Machine/WallRunContext.h"
#include "FlowMotionFactory.generated.h"

UCLASS()
class FLOWMOTION_API UFlowMotionFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Creates a new motion machine instance.
	 * @param InContext The context to use for the motion machine.
	 * @return A new instance of the motion machine.
	 */
	static UMotionMachine* CreateMotionMachine(UMotionContext* InContext = nullptr);

	/**
	 * @brief Creates a new motion state instance.
	 * @param Outer The outer object for the new motion state.
	 * @param InStateClass The class of the motion state to create.
	 * @return A new instance of the motion state.
	 */
	static UMotionState* CreateMotionState(UObject* Outer, const TSubclassOf<UMotionState>& InStateClass);

	/**
	 * @brief Creates a new wall run context instance.
	 * @param InWallRunner The wall runner to use for the context.
	 * @param InMovementComponent The movement component to use for the context.
	 * @return A new instance of the wall run context.
	 */
	static UWallRunContext* CreateWallRunContext(UWallRunner* InWallRunner, UCharacterMovementComponent* InMovementComponent);

	/**
	 * @brief Creates a new rail grind context instance.
	 * @param InRailGrinder The rail grinder to use for the context.
	 * @param InMovementComponent The movement component to use for the context.
	 * @return A new instance of the rail grind context.
	 */
	static URailGrindContext* CreateRailGrindContext(URailGrinder* InRailGrinder, UCharacterMovementComponent* InMovementComponent);
};
