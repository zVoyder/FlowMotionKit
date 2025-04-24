// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindAttachmentState.h"

void URailGrindAttachmentState::OnEnter()
{
	Super::OnEnter();
	ElapsedTime = 0.f;

	SetDirection();
	URailGrindContext* Context = GetRailGrindContext();
	// Context->MovementComponent->bOrientRotationToMovement = false;
	Context->MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	Context->MovementComponent->GravityScale = 0.f;
	Context->RailGrinder->TryGetMostValidRailHit(Context->HitData);
}

void URailGrindAttachmentState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	ElapsedTime += DeltaTime;
	
	// AttachCharacterOnRail();
	if (ElapsedTime >= GetRailGrindContext()->RailGrinder->AttachmentDuration)
		TransitionTo(RailGrindGrindingStateName);
}

void URailGrindAttachmentState::OnExit()
{
	Super::OnExit();
}

void URailGrindAttachmentState::OnAbort()
{
	Super::OnAbort();
	GetRailGrindContext()->RailGrinder->ResetMovementComponentData();
}

void URailGrindAttachmentState::SetDirection() const
{
	URailGrindContext* Context = GetRailGrindContext();
	const FVector ForwardVector = Context->Owner->GetActorForwardVector();
	const FVector SplineTangent = Context->HitData.Rail->GetRailSplineComponent()->GetTangentAtDistanceAlongSpline(
		Context->HitData.Rail->GetClosestDistanceOnSpline(Context->HitData.HitResult.ImpactPoint),
		ESplineCoordinateSpace::World
	).GetSafeNormal();
	
	Context->bIsGoingReverse = FVector::DotProduct(ForwardVector, SplineTangent) < 0;
}

void URailGrindAttachmentState::AttachCharacterOnRail() const
{
	bool bIsGoingReverse = GetRailGrindContext()->bIsGoingReverse;
	const URailGrinder* RailGrinder = GetRailGrindContext()->RailGrinder;
	const FRailHitData& RailHitData = GetRailGrindContext()->HitData;

	const USplineComponent* Spline = RailHitData.Rail->GetRailSplineComponent();
	const float ClosestDistance = RailHitData.Rail->GetClosestDistanceOnSpline(GetRailGrindContext()->Owner->GetActorLocation());

	FVector TargetLocation = Spline->GetLocationAtDistanceAlongSpline(ClosestDistance, ESplineCoordinateSpace::World);
	FRotator TargetRotation = Spline->GetRotationAtDistanceAlongSpline(ClosestDistance, ESplineCoordinateSpace::World).GetNormalized();

	if (bIsGoingReverse)
	{
		TargetRotation = TargetRotation + FRotator(0.f, 180.f, 0.f);
		TargetRotation.Pitch *= -1.f;
	}

	const FVector Offset = FVector(0.f, 0.f, RailGrinder->RailOffset);
	TargetLocation += Offset;

	float Duration = RailGrinder->AttachmentDuration;
	const FVector CurrentLocation = GetRailGrindContext()->Owner->GetActorLocation();

	const float Alpha = FMath::Clamp(ElapsedTime / RailGrinder->AttachmentDuration, 0.f, 1.f);
	const FVector FinalLocation = FMath::Lerp(CurrentLocation, TargetLocation, Alpha);
	const FQuat FinalRotation = FQuat::Slerp(GetRailGrindContext()->Owner->GetActorRotation().Quaternion(), TargetRotation.Quaternion(), Alpha);
	GetRailGrindContext()->Owner->SetActorLocation(FinalLocation);
	GetRailGrindContext()->Owner->SetActorRotation(FinalRotation.Rotator());
}
