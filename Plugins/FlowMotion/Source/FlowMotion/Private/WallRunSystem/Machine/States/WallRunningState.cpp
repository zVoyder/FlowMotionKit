// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunningState.h"

void UWallRunningState::OnAdded(UMotionMachine* InMachine)
{
	Super::OnAdded(InMachine);
	MovementComponent = GetWallRunContext()->MovementComponent;
}

void UWallRunningState::OnEnter()
{
	Super::OnEnter();

	ElapsedTime = 0.f;
	OriginalGravityScale = MovementComponent->GravityScale;
	bOriginalOrientRotationToMovement = MovementComponent->bOrientRotationToMovement;
	MovementComponent->bOrientRotationToMovement = false;

	SetGravityCurve();
	const FVector& HorVelocity = GetWallRunContext()->StartUpHorVelocity;
	MovementComponent->Velocity = HorVelocity; // Set vertical velocity to zero
}

void UWallRunningState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	ElapsedTime += DeltaTime;
	
	UWallRunContext* Context = GetWallRunContext();
	if (!Context->Runner->WantsToAttach())
	{
		TransitionTo(FallingStateName);
		return;
	}

	const float& MinVelocity = Context->MovementComponent->Velocity.Length();
	const bool& bCheckRight = Context->bLastWallIsRight;

	FHitResult HitResult;
	if (!Context->Runner->CheckAttachWall(MinVelocity, bCheckRight, HitResult, Context->LastWall))
	{
		TransitionTo(FallingStateName);
		return;
	}
	
	const FRotator WallOrientation = GetWallOrientation(bCheckRight, HitResult);
	RotateCharacterAlongWall(DeltaTime, WallOrientation);
	MoveCharacterAlongWall(DeltaTime, HitResult, WallOrientation);
	ScaleGravityWithCurve();
}

void UWallRunningState::OnExit()
{
	Super::OnExit();
	ResetMovementComponent();
}

void UWallRunningState::OnAbort()
{
	Super::OnAbort();
	ResetMovementComponent();
}

void UWallRunningState::RotateCharacterAlongWall(const float DeltaTime, const FRotator& WallOrientation) const
{
	const FRotator CurrentRotation = GetWallRunContext()->Owner->GetActorRotation();
	const FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, WallOrientation, DeltaTime, 10.f);
	GetWallRunContext()->Owner->SetActorRotation(InterpRotation);
}

void UWallRunningState::MoveCharacterAlongWall(const float DeltaTime, const FHitResult& HitResult, const FRotator& WallOrientation) const
{
	AActor* Owner = GetWallRunContext()->Owner;
	const float& DesiredDistance = GetWallRunContext()->Runner->DesiredDistanceToWall;

	const FVector ImpactNormal = HitResult.ImpactNormal.GetSafeNormal();
	const FVector WallForward = WallOrientation.Vector();
	const FVector WallNormal = -ImpactNormal.GetSafeNormal();
	const FVector FinalDirection = (WallForward + WallNormal).GetSafeNormal();

	MovementComponent->AddInputVector(FinalDirection);

	const FVector ToWall = HitResult.ImpactPoint - Owner->GetActorLocation();
	const float Distance = FVector::DotProduct(ToWall, ImpactNormal);
	const FVector Correction = ImpactNormal * (Distance + DesiredDistance);
	Owner->AddActorWorldOffset(Correction * DeltaTime, true);
}

void UWallRunningState::ScaleGravityWithCurve() const
{
	if (!IsValid(GravityCurve))
		return;
	
	const float Mult = GravityCurve->GetFloatValue(ElapsedTime);
	MovementComponent->GravityScale = OriginalGravityScale * Mult;
}

void UWallRunningState::SetGravityCurve()
{
	GravityCurve = GetWallRunContext()->LastWall->WallGravityMultiplierCurve;
	
	if (!IsValid(GravityCurve))
		GravityCurve = GetWallRunContext()->Runner->DefaultGravityMultiplierCurve;
}

FRotator UWallRunningState::GetWallOrientation(const bool bIsRight, const FHitResult& HitResult) const
{
	const AActor* Owner = GetWallRunContext()->Owner;
	const FVector ImpactNormal = HitResult.ImpactNormal.GetSafeNormal();
	FVector WallForward =
		bIsRight
			? FVector::CrossProduct(Owner->GetActorUpVector(), ImpactNormal).GetSafeNormal()
			: FVector::CrossProduct(ImpactNormal, Owner->GetActorUpVector()).GetSafeNormal();

	if (WallForward.IsNearlyZero())
		WallForward = Owner->GetActorForwardVector(); // fallback

	return FRotationMatrix::MakeFromXZ(WallForward, Owner->GetActorUpVector()).Rotator();
}

void UWallRunningState::ResetMovementComponent() const
{
	MovementComponent->bOrientRotationToMovement = bOriginalOrientRotationToMovement;
	MovementComponent->GravityScale = OriginalGravityScale;
}
