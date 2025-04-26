// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindGrindingState.h"

void URailGrindGrindingState::OnEnter()
{
	Super::OnEnter();
	// UpdateDistanceAlongSpline();
}

void URailGrindGrindingState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	URailGrindContext* Context = GetRailGrindContext();

	if (HasReachedEndOfRail())
	{
		GetMachine()->StopMachine();
		return;
	}

	URailGrinder* RailGrinder = Context->RailGrinder;
	RailGrinder->MoveAndRotateCharacterAlongRail(DeltaTime, Context->SplineDistance, Context->HitData);
}

void URailGrindGrindingState::OnExit()
{
	Super::OnExit();
}

void URailGrindGrindingState::OnAbort()
{
	Super::OnAbort();
	GetRailGrindContext()->RailGrinder->ResetMovementComponentData();
	GetRailGrindContext()->RailGrinder->OnRailGrindDetach.Broadcast();
	LaunchCharacterOffRail();
}

void URailGrindGrindingState::UpdateDistanceAlongSpline() const
{
	URailGrindContext* Context = GetRailGrindContext();
	Context->SplineDistance = Context->HitData.Rail->GetClosestDistanceOnSpline(Context->HitData.HitResult.ImpactPoint);
}

void URailGrindGrindingState::LaunchCharacterOffRail() const
{
	const URailGrindContext* Context = GetRailGrindContext();
	const URailGrinder* Grinder = Context->RailGrinder;

	FVector SplineTangent = Context->HitData.Rail->GetSplineComponent()
	                               ->GetTangentAtDistanceAlongSpline(Context->SplineDistance, ESplineCoordinateSpace::World)
	                               .GetSafeNormal();

	if (Context->HitData.bIsGoingReverse)
		SplineTangent = -SplineTangent;

	const FVector CurrentVelocity = Context->MovementComponent->Velocity;
	const FVector HorizontalVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.f);
	const FVector ForwardMomentum = SplineTangent * HorizontalVelocity.Size() * Grinder->ForwardLaunchScale;

	const FVector LaunchVector = (Context->Owner->GetActorUpVector() * Grinder->VerticalLaunchForce) + (SplineTangent * Grinder->HorizontalLaunchForce) + ForwardMomentum;
	Context->MovementComponent->Launch(LaunchVector);
	Grinder->OnRailGrindLaunch.Broadcast();
}

bool URailGrindGrindingState::HasReachedEndOfRail() const
{
	const URailGrindContext* Context = GetRailGrindContext();
	const USplineComponent* Spline = Context->HitData.Rail->GetSplineComponent();

	if (!Context->HitData.bIsGoingReverse)
	{
		const float SplineLength = Spline->GetSplineLength();
		return FMath::IsNearlyEqual(Context->SplineDistance, SplineLength, GetDetachDistance());
	}

	return FMath::IsNearlyEqual(Context->SplineDistance, 0.f, GetDetachDistance());
}

float URailGrindGrindingState::GetDetachDistance() const
{
	const URailGrindContext* Context = GetRailGrindContext();

	if (Context->HitData.Rail->HasDetachDistanceOverride())
		return Context->HitData.Rail->DetachDistanceOverride;

	return Context->RailGrinder->DefaultDetachDistance;
}
