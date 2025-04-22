// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunStateBase.h"

void UWallRunStateBase::OnAdded(UMotionMachine* InMachine)
{
	Super::OnAdded(InMachine);
	WallRunContext = Cast<UWallRunContext>(GetMachineContext());
}

UWallRunContext* UWallRunStateBase::GetWallRunContext() const
{
	return WallRunContext;
}
