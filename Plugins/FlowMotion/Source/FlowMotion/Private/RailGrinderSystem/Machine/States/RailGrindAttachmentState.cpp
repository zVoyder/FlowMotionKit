// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindAttachmentState.h"

void URailGrindAttachmentState::OnEnter()
{
	Super::OnEnter();
	ElapsedTime = 0.f;
	
	StartLocation = GetRailGrindContext()->Owner->GetActorLocation();
	StartRotation = GetRailGrindContext()->Owner->GetActorRotation();

	const URailGrindContext* Context = GetRailGrindContext();
	Context->MovementComponent->bOrientRotationToMovement = false;
	Context->MovementComponent->DisableMovement();
	Context->RailGrinder->OnRailGrindAttach.Broadcast();
	SetAttachPoint();
}

void URailGrindAttachmentState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	ElapsedTime += DeltaTime;

	if (IsAttached())
	{
		TransitionTo(RailGrindGrindingStateName);
		return;
	}

	InterpToAttachPointAndRotation();
}

void URailGrindAttachmentState::OnExit()
{
	Super::OnExit();
}

void URailGrindAttachmentState::OnAbort()
{
	Super::OnAbort();
	GetRailGrindContext()->RailGrinder->ResetMovementComponentData();
	GetRailGrindContext()->RailGrinder->OnRailGrindDetach.Broadcast();
}

void URailGrindAttachmentState::SetAttachPoint()
{
	URailGrindContext* Context = GetRailGrindContext();
	const FRailHitData RailHitData = Context->HitData;

	const USplineComponent* Spline = RailHitData.Rail->GetSplineComponent();
	Context->SplineDistance = Context->HitData.Rail->GetClosestDistanceOnSpline(Context->HitData.HitResult.ImpactPoint);
	const FVector SplineLocation = Spline->GetLocationAtDistanceAlongSpline(Context->SplineDistance, ESplineCoordinateSpace::World);
	const FVector Offset = Context->Owner->GetActorUpVector() * Context->RailGrinder->GetRailOffset(RailHitData.Rail);
	AttachRotation = Spline->GetRotationAtDistanceAlongSpline(Context->SplineDistance, ESplineCoordinateSpace::World);
	if (Context->HitData.bIsGoingReverse)
	{
		AttachRotation.Yaw += 180.f;
		AttachRotation.Pitch *= -1.f;
		AttachRotation.Roll *= -1.f;
	}

	AttachPoint = SplineLocation + Offset;
}

void URailGrindAttachmentState::InterpToAttachPointAndRotation() const
{
	const URailGrindContext* Context = GetRailGrindContext();
	const FVector FinalLocation = FMath::Lerp(StartLocation, AttachPoint, ElapsedTime / Context->RailGrinder->AttachmentDuration);
	const FRotator FinalRotation = FMath::Lerp(StartRotation, AttachRotation, ElapsedTime / Context->RailGrinder->AttachmentDuration);
	Context->Owner->SetActorLocationAndRotation(FinalLocation, FinalRotation, false);
}

bool URailGrindAttachmentState::IsAttached() const
{
	const URailGrindContext* Context = GetRailGrindContext();

	return ElapsedTime >= Context->RailGrinder->AttachmentDuration;
}
