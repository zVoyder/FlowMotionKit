// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunInputState.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunInputState : public UWallRunStateBase
{
	GENERATED_BODY()

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;
	
	virtual void OnExit() override;
};
