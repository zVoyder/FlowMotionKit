// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RailGrinderSystem/Machine/States/RailGrindStateBase.h"
#include "RailGrindCheckState.generated.h"

UCLASS()
class FLOWMOTION_API URailGrindCheckState : public URailGrindStateBase
{
	GENERATED_BODY()

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

private:
	/**
	 * @brief Checks if the character has sufficient speed to grind.
	 * @return True if the character has sufficient speed to grind, false otherwise.
	 */
	bool HasSufficientSpeedToGrind() const;
};
