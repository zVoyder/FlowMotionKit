// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunningState.h"
#include "FlowMotion.h"

void UWallRunningState::OnEnter()
{
	Super::OnEnter();
	ElapsedTime = 0.f;

	SetGravity();
	SetSpeedAcceleration();
}

void UWallRunningState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	ElapsedTime += DeltaTime;

	ScaleGravityWithCurve();
	ScaleSpeedAccelerationWithCurve();

	const UWallRunContext* Context = GetWallRunContext();
	const UWallRunner* Runner = Context->Runner;
	if (!Runner->WantsToAttach())
	{
		LaunchCharacterOffWall();
		TransitionTo(FallingStateName);
		return;
	}

	if (!HasWallOnSide())
	{
		TransitionTo(FallingStateName);
		return;
	}

	const FRotator WallOrientation = Runner->GetWallOrientation(Context->HitData);
	Runner->RotateCharacterAlongWall(DeltaTime, WallOrientation);
	const float Stickiness = Runner->GetStickinessStrength(Context->HitData.Wall);
	Runner->MoveCharacterAlongWall(DeltaTime, Context->HitData.HitResult, WallOrientation, Stickiness);

	if (ElapsedTime >= Runner->MinWallRunTime)
	{
		if (!HasSufficientSpeedToKeepRunning())
		{
			UE_LOG(LogFlowMotion, Warning, TEXT("WallRunningState::OnProcess: Insufficient speed."));
			TransitionTo(FallingStateName);
		}
	}
}

void UWallRunningState::OnExit()
{
	Super::OnExit();
}

void UWallRunningState::OnAbort()
{
	Super::OnAbort();
	GetWallRunContext()->Runner->ResetMovementComponentData(); // Safe call
}

bool UWallRunningState::HasWallOnSide() const
{
	UWallRunContext* Context = GetWallRunContext();
	const AActor* Owner = Context->Owner;
	const UWallRunner* Runner = Context->Runner;

	if (!IsValid(Owner))
		return false;

	if (!Context->Runner->TryGetMostValidWallHit(Context->HitData))
		return false;

	const UWorld* World = Owner->GetWorld();

	if (!IsValid(World))
		return false;

	const FVector TraceStartPoint = Runner->GetActorTraceLocation();
	const FVector Direction = (Owner->GetActorRightVector() * (Context->HitData.bIsOnRight ? 1.f : -1.f)).GetSafeNormal();
	const FVector TraceEndPoint = TraceStartPoint + Runner->CheckRadius * TraceDistanceMultiplier * Direction;

	FHitResult HitResult;
	World->LineTraceSingleByChannel(HitResult, TraceStartPoint, TraceEndPoint, Runner->TraceCheckChannel);
	
	return HitResult.bBlockingHit;
}

bool UWallRunningState::HasSufficientSpeedToKeepRunning() const
{
	const FVector HorizontalVelocity = FVector(
		GetWallRunContext()->MovementComponent->Velocity.X,
		GetWallRunContext()->MovementComponent->Velocity.Y,
		0.f
	);

	return HorizontalVelocity.Length() > GetWallRunContext()->Runner->VelocityToDetach;
}

void UWallRunningState::SetGravity()
{
	const UWallRunner* Runner = GetWallRunContext()->Runner;
	
	if (!GetWallRunContext()->Runner->bUseGravityCurves)
	{
		bUseGravityCurve = false;
		GetWallRunContext()->MovementComponent->GravityScale = Runner->DefaultGravityScale;
		return;
	}
	
	const URunnableWall* Wall = GetWallRunContext()->HitData.Wall;
	if (Wall->HasWallGravityCurveOverride())
	{
		bUseGravityCurve = true;
		GravityCurve = GetWallRunContext()->HitData.Wall->WallGravityCurveOverride;
		return;
	}
	
	GravityCurve = GetWallRunContext()->Runner->DefaultGravityMultiplierCurve;
	if (IsValid(GravityCurve))
		bUseGravityCurve = true;
}

void UWallRunningState::ScaleGravityWithCurve() const
{
	if (!bUseGravityCurve)
		return;

	const UWallRunner* Runner = GetWallRunContext()->Runner;
	const float Mult = GravityCurve->GetFloatValue(ElapsedTime);

	GetWallRunContext()->MovementComponent->GravityScale = Runner->GetOriginalGravityScale() * Mult;
}

void UWallRunningState::SetSpeedAcceleration()
{
	const UWallRunner* Runner = GetWallRunContext()->Runner;
	
	if (!GetWallRunContext()->Runner->bUseSpeedAccelerationCurves)
	{
		bUseSpeedAccelerationCurve = false;
		GetWallRunContext()->MovementComponent->MaxWalkSpeed *= Runner->DefaultMaxSpeedScale;
		GetWallRunContext()->MovementComponent->MaxAcceleration *= Runner->DefaultAccelerationScale;
		return;
	}

	ContactSpeed = GetWallRunContext()->MovementComponent->MaxWalkSpeed;
	ContactAcceleration = GetWallRunContext()->MovementComponent->MaxAcceleration;
	const URunnableWall* Wall = GetWallRunContext()->HitData.Wall;
	if (Wall->HasWallSpeedAccelerationCurveOverride())
	{
		bUseSpeedAccelerationCurve = true;
		SpeedAccelerationCurve = GetWallRunContext()->HitData.Wall->WallSpeedAccelerationCurveOverride;
		return;
	}

	SpeedAccelerationCurve = GetWallRunContext()->Runner->DefaultSpeedAccelerationCurve;
	if (IsValid(SpeedAccelerationCurve))
		bUseSpeedAccelerationCurve = true;
}

void UWallRunningState::ScaleSpeedAccelerationWithCurve() const
{
	if (!bUseSpeedAccelerationCurve)
		return;
	
	const UWallRunner* Runner = GetWallRunContext()->Runner;
	const float SpeedMult = SpeedAccelerationCurve->GetVectorValue(ElapsedTime).X;
	const float AccelerationMult = SpeedAccelerationCurve->GetVectorValue(ElapsedTime).Y;
	
	GetWallRunContext()->MovementComponent->MaxWalkSpeed = ContactSpeed * SpeedMult;
	GetWallRunContext()->MovementComponent->MaxAcceleration = ContactAcceleration * AccelerationMult;
}

void UWallRunningState::LaunchCharacterOffWall() const
{
	const UWallRunContext* Context = GetWallRunContext();
	const UWallRunner* Runner = Context->Runner;
	const UCharacterMovementComponent* MovementComponent = Context->MovementComponent;

	if (!IsValid(Runner) || !IsValid(MovementComponent))
		return;

	const FVector ImpactNormal = Context->HitData.HitResult.ImpactNormal.GetSafeNormal();
	const FVector WallForward = Runner->GetWallOrientation(Context->HitData).Vector();

	const FVector CurrentVelocity = MovementComponent->Velocity;
	const FVector HorizontalVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.f);
	const FVector ForwardMomentum = WallForward * HorizontalVelocity.Size() * Runner->ForwardLaunchScale;

	const FVector LaunchVector = (ImpactNormal * Runner->HorizontalLaunchBoost) + (FVector::UpVector * Runner->VerticalLaunchBoost) + ForwardMomentum;
	Context->MovementComponent->Launch(LaunchVector);
}
