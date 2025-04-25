// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunAttachmentState.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunAttachmentState : public UWallRunStateBase
{
	GENERATED_BODY()

private:
	float ElapsedTime = 0.f;
	
protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;
	
	virtual void OnAbort() override;

private:
	/**
	 * @brief Detaches the character from the wall.
	 */
	void Detach() const;

	/**
	 * @brief Scales the vertical velocity (Z) of the character.
	 * @param Scale The scale factor to apply to the vertical velocity (Z).
	 */
	void ScaleVerticalVelocity(const float Scale = 0.f) const;
};
