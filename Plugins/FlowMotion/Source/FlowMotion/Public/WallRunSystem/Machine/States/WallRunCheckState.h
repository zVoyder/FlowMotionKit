// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunCheckState.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunCheckState : public UWallRunStateBase
{
	GENERATED_BODY()

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

private:
	/**
	 * @brief Checks if the character can attach to the wall.
	 * @param WallHitData The wall hit data to check against.
	 * @return True if the character can attach to the wall, false otherwise.
	 */
	bool CanAttachToWall(const FWallHitData& WallHitData) const;

	/**
	 * @brief Checks if the character has sufficient speed to attach to the wall.
	 * @return True if the character has sufficient speed to attach, false otherwise.
	 */
	bool HasSufficientSpeedToAttach() const;
};
