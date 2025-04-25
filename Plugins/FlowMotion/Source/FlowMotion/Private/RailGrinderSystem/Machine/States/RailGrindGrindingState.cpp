// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindGrindingState.h"

void URailGrindGrindingState::OnEnter()
{
	Super::OnEnter();

	SetDirection();
	UpdateDistanceAlongSpline();

	const URailGrindContext* Context = GetRailGrindContext();
	Context->MovementComponent->bOrientRotationToMovement = false;
	Context->MovementComponent->DisableMovement();
	Context->RailGrinder->OnRailGrindAttach.Broadcast();
}

void URailGrindGrindingState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);

	URailGrindContext* Context = GetRailGrindContext();
	URailGrinder* RailGrinder = Context->RailGrinder;

	RailGrinder->MoveAndRotateCharacterAlongRail(DeltaTime, DistanceAlongSpline, Context->HitData, Context->bIsGoingReverse);

	if (HasReachedEndOfRail())
		GetMachine()->StopMachine();
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

void URailGrindGrindingState::SetDirection() const
{
	URailGrindContext* Context = GetRailGrindContext();
	const FVector ForwardVector = Context->Owner->GetActorForwardVector();
	const FVector SplineTangent = Context->HitData.Rail->GetSplineComponent()->GetTangentAtDistanceAlongSpline(
		Context->HitData.Rail->GetClosestDistanceOnSpline(Context->HitData.HitResult.ImpactPoint),
		ESplineCoordinateSpace::World
	).GetSafeNormal();

	Context->bIsGoingReverse = FVector::DotProduct(ForwardVector, SplineTangent) < 0;
}

void URailGrindGrindingState::UpdateDistanceAlongSpline()
{
	const URailGrindContext* Context = GetRailGrindContext();
	DistanceAlongSpline = Context->HitData.Rail->GetClosestDistanceOnSpline(Context->HitData.HitResult.ImpactPoint);
}

void URailGrindGrindingState::LaunchCharacterOffRail() const
{
	const URailGrindContext* Context = GetRailGrindContext();
	const URailGrinder* Grinder = Context->RailGrinder;
	
	FVector SplineTangent = Context->HitData.Rail->GetSplineComponent()
		->GetTangentAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World)
		.GetSafeNormal();

	if (Context->bIsGoingReverse)
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

	if (!Context->bIsGoingReverse)
	{
		const float SplineLength = Spline->GetSplineLength();
		return FMath::IsNearlyEqual(DistanceAlongSpline, SplineLength, GetDetachDistance());
	}

	return FMath::IsNearlyEqual(DistanceAlongSpline, 0.f, GetDetachDistance());
}

float URailGrindGrindingState::GetDetachDistance() const
{
	const URailGrindContext* Context = GetRailGrindContext();

	if (Context->HitData.Rail->HasDetachDistanceOverride())
		return Context->HitData.Rail->DetachDistanceOverride;

	return Context->RailGrinder->DefaultDetachDistance;
}
