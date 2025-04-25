// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MotionStateMachine/MotionState.h"
#include "WallRunSystem/Machine/WallRunContext.h"
#include "WallRunStateBase.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunStateBase : public UMotionState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UWallRunContext* WallRunContext;
	
protected:
	virtual void OnAdded(UMotionMachine* InMachine) override;

	/**
	 * @brief Gets the wall run context.
	 * @return The wall run context.
	 */
	UWallRunContext* GetWallRunContext() const;
};
