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
	static UMotionMachine* CreateMotionMachine(UMotionContext* InContext = nullptr);

	static UMotionState* CreateMotionState(UObject* Outer, const TSubclassOf<UMotionState>& InStateClass);

	static UWallRunContext* CreateWallRunContext(UWallRunner* InWallRunner, UCharacterMovementComponent* InMovementComponent);

	static URailGrindContext* CreateRailGrindContext(URailGrinder* InRailGrinder, UCharacterMovementComponent* InMovementComponent);
};
