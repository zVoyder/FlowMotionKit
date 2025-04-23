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
	void SetVerticalVelocity(const float ZVelocity = 0.f) const;
};
