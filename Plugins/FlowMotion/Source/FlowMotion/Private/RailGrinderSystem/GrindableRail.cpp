// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/GrindableRail.h"
#include "FlowMotion.h"

UGrindableRail::UGrindableRail(): SplineComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UGrindableRail::GetClosestDistanceOnSpline(const FVector& WorldLocation) const
{
	if (!Check())
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("UGrindableRail::GetClosestDistanceOnSpline: Check failed."));
		return 0.f;
	}
	
	const float InputKey = SplineComponent->FindInputKeyClosestToWorldLocation(WorldLocation);
	const float Distance = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(InputKey);
	return Distance;
}

USplineComponent* UGrindableRail::GetSplineComponent() const
{
	return SplineComponent;
}

bool UGrindableRail::HasRailOffsetOverride() const
{
	return bOverrideRailOffset;
}

bool UGrindableRail::HasRailSpeedOverride() const
{
	return bOverrideSpeed;
}

bool UGrindableRail::HasDetachDistanceOverride() const
{
	return bOverrideDetachDistance;
}

void UGrindableRail::BeginPlay()
{
	Super::BeginPlay();
	SplineComponent =  GetOwner()->FindComponentByClass<USplineComponent>();
}

bool UGrindableRail::Check() const
{
	return IsValid(SplineComponent);
}
