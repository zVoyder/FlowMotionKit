// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MotionStateMachine/MotionState.h"
#include "RailGrinderSystem/Machine/RailGrindContext.h"
#include "RailGrindStateBase.generated.h"

UCLASS()
class FLOWMOTION_API URailGrindStateBase : public UMotionState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	URailGrindContext* WallRunContext;
	
protected:
	virtual void OnAdded(UMotionMachine* InMachine) override;

	/**
	 * @brief Gets the rail grind context.
	 * @return The rail grind context.
	 */
	URailGrindContext* GetRailGrindContext() const;
};
