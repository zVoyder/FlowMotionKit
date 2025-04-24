// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindStateBase.h"

void URailGrindStateBase::OnAdded(UMotionMachine* InMachine)
{
	Super::OnAdded(InMachine);
	WallRunContext = Cast<URailGrindContext>(InMachine->GetContext());
}

URailGrindContext* URailGrindStateBase::GetRailGrindContext() const
{
	return WallRunContext;
}
