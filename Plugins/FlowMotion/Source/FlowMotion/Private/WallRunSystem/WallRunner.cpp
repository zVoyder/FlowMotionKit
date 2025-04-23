// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/WallRunner.h"
#include "FlowMotion.h"
#include "Data/WallHitData.h"
#include "Factories/FlowMotionFactory.h"
#include "WallRunSystem/Machine/WallRunContext.h"
#include "WallRunSystem/Machine/States/WallRunAttachmentState.h"
#include "WallRunSystem/Machine/States/WallRunFallingState.h"
#include "WallRunSystem/Machine/States/WallRunCheckState.h"
#include "WallRunSystem/Machine/States/WallRunInitState.h"
#include "WallRunSystem/Machine/States/WallRunInputState.h"
#include "WallRunSystem/Machine/States/WallRunningState.h"

UWallRunner::UWallRunner(): bUseGravityCurves(false),
                            DefaultGravityMultiplierCurve(nullptr),
                            bUseSpeedAccelerationCurves(false),
                            DefaultSpeedAccelerationCurve(nullptr),
                            PivotOffset(),
                            MotionMachine(nullptr),
                            Owner(nullptr),
                            CharacterMovementComponent(nullptr),
                            OriginalGravityScale(0),
                            OriginalSpeedScale(0),
                            OriginalAccelerationScale(0),
                            bOriginalOrientRotationToMovement(false),
                            LaunchDirection()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWallRunner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CharacterMovementComponent->IsFalling())
		MotionMachine->StartMachine(InitStateName); // Avoid starting the machine if character is not falling
	else
		MotionMachine->StopMachine();
}

void UWallRunner::AttachToWall()
{
	bWantsToAttach = true;
}

void UWallRunner::DetachFromWall()
{
	bWantsToAttach = false;
}

void UWallRunner::RotateCharacterAlongWall(const float DeltaTime, const FRotator& WallOrientation) const
{
	const FRotator CurrentRotation = Owner->GetActorRotation();
	const FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, WallOrientation, DeltaTime, RotationInterpSpeed);
	Owner->SetActorRotation(InterpRotation);
}

void UWallRunner::MoveCharacterAlongWall(const float DeltaTime, const FHitResult& HitResult, const FRotator& WallOrientation, const float StickinessStrength) const
{
	const FVector ImpactNormal = HitResult.ImpactNormal.GetSafeNormal();
	const FVector WallForward = WallOrientation.Vector();
	const FVector WallNormal = -ImpactNormal.GetSafeNormal();

	const float RawSpeed = CharacterMovementComponent->Velocity.Size();
	const float SpeedMultiplier = FMath::Loge(RawSpeed + 1.f);
	
	CharacterMovementComponent->AddImpulse(WallNormal * StickinessStrength * SpeedMultiplier, true); // Keep the character on the wall
	CharacterMovementComponent->AddInputVector(WallForward); // Move the character along the wall
}

bool UWallRunner::IsWallRunning() const
{
	if (!IsValid(MotionMachine) || !MotionMachine->IsRunning())
		return false;
	
	return MotionMachine->IsStateActive(RunningStateName) || 
	       MotionMachine->IsStateActive(AttachStateName);
}

bool UWallRunner::IsWallRunningOnRight() const
{
	if (!IsWallRunning())
		return false;

	const UWallRunContext* WallRunContext = Cast<UWallRunContext>(MotionMachine->GetContext());
	if (!IsValid(WallRunContext))
		return false;
	
	return WallRunContext->HitData.bIsOnRight;
}

bool UWallRunner::IsAttachingToWall() const
{
	return MotionMachine->IsStateActive(AttachStateName);
}

bool UWallRunner::WantsToAttach() const
{
	return bWantsToAttach;
}

bool UWallRunner::TryGetMostValidWallHit(FWallHitData& OutWallHitData) const
{
	TMap<URunnableWall*, FHitResult> HitsMap = GetWallsHitsMap();
	float BestScore = MAX_FLT;

	if (!IsValid(Owner) || HitsMap.IsEmpty())
		return false;

	const FVector PlayerLocation = Owner->GetActorLocation();
	const FVector PlayerRight = Owner->GetActorRightVector();

	for (const auto& HitPair : HitsMap)
	{
		URunnableWall* Wall = HitPair.Key;
		const FHitResult& HitResult = HitPair.Value;

		if (!IsValid(Wall))
			continue;

		// Distance from player to wall
		const float Distance = FVector::Dist(PlayerLocation, HitResult.ImpactPoint);

		// Vertical angle from player to wall
		const FVector ToHit = (HitResult.ImpactPoint - PlayerLocation).GetSafeNormal();
		const float VerticalAngle = FMath::Abs(FMath::RadiansToDegrees(FMath::Asin(ToHit.Z)));

		if (VerticalAngle > MaxVerticalAngleDifference)
			continue;

		// Horizontal angle from player to wall
		FVector ToHitHoriz = ToHit;
		ToHitHoriz.Z = 0.f;
		ToHitHoriz.Normalize();
		float HorizontalAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PlayerRight, ToHitHoriz)));
		const bool bIsOnRight = HorizontalAngle < 90.f;

		HorizontalAngle = bIsOnRight ? HorizontalAngle : 180.f - HorizontalAngle;
		if (HorizontalAngle > MaxHorizontalAngleDifference)
			continue;

		const float Score = Distance + VerticalAngle + HorizontalAngle;

		if (Score < BestScore)
		{
			BestScore = Score;
			OutWallHitData.bIsOnRight = bIsOnRight;
			OutWallHitData.Wall = Wall;
			OutWallHitData.HitResult = HitResult;
		}
	}

	if (!OutWallHitData.IsWallHitValid())
		return false;

#if WITH_EDITORONLY_DATA
	if (bShowDebug)
	{
		DrawDebugBox(
			GetWorld(),
			OutWallHitData.HitResult.ImpactPoint,
			FVector(10.f),
			FQuat::Identity,
			FColor::Green,
			false,
			0,
			0,
			2.0f
		);
	}
#endif

	return true;
}

TMap<URunnableWall*, FHitResult> UWallRunner::GetWallsHitsMap() const
{
	if (!Check())
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("WallRunner::GetWallsHitsMap: Check failed."));
		return {};
	}

	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("WallRunner::GetWallsHitsMap: World is not valid."));
		return {};
	}

	TArray<FHitResult> HitResults;
	World->SweepMultiByChannel(
		HitResults,
		GetActorTraceLocation(),
		GetActorTraceLocation(),
		FQuat::Identity,
		TraceCheckChannel,
		FCollisionShape::MakeSphere(CheckRadius)
	);

#if WITH_EDITORONLY_DATA
	if (bShowDebug)
	{
		DrawDebugSphere(
			World,
			Owner->GetActorLocation(),
			CheckRadius,
			12,
			FColor::Green,
			false,
			0,
			0,
			2.0f
		);
	}
#endif

	TMap<URunnableWall*, FHitResult> HitsMap;
	for (const FHitResult& HitResult : HitResults)
	{
		if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
		{
			URunnableWall* Wall = HitResult.GetActor()->FindComponentByClass<URunnableWall>();
			if (!IsValid(Wall))
				continue;

			HitsMap.Add(Wall, HitResult);

#if WITH_EDITORONLY_DATA
			if (bShowDebug)
			{
				DrawDebugLine(
					World,
					Owner->GetActorLocation(),
					HitResult.ImpactPoint,
					FColor::Blue,
					false,
					0,
					0,
					2.0f
				);
			}
#endif
		}
	}

	return HitsMap;
}

float UWallRunner::GetStickinessStrength(const URunnableWall* Wall) const
{
	if (!IsValid(Wall) || !Wall->HasStickinessStrengthOverride())
		return DefaultStickinessStrength;
	
	return Wall->StickinessStrengthOverride;
}

FVector UWallRunner::GetActorTraceLocation() const
{
	return Owner->GetActorLocation() + PivotOffset;
}

void UWallRunner::SetOriginalGravityScale(const float GravityScale)
{
	OriginalGravityScale = GravityScale;
}

void UWallRunner::SetOriginalSpeed(const float SpeedScale)
{
	OriginalSpeedScale = SpeedScale;
}

void UWallRunner::SetOriginalAcceleration(const float AccelerationScale)
{
	OriginalAccelerationScale = AccelerationScale;
}

float UWallRunner::GetOriginalGravityScale() const
{
	return OriginalGravityScale;
}

float UWallRunner::GetOriginalSpeed() const
{
	return OriginalSpeedScale;
}

float UWallRunner::GetOriginalAcceleration() const
{
	return OriginalAccelerationScale;
}

bool UWallRunner::GetOriginalOrientRotationToMovement() const
{
	return bOriginalOrientRotationToMovement;
}

void UWallRunner::ResetMovementComponentData() const
{
	ResetGravityScale();
	ResetSpeed();
	ResetAcceleration();
	ResetOrientationToMovement();
}

void UWallRunner::ResetGravityScale() const
{
	CharacterMovementComponent->GravityScale = OriginalGravityScale;
}

void UWallRunner::ResetSpeed() const
{
	CharacterMovementComponent->MaxWalkSpeed = OriginalSpeedScale;
}

void UWallRunner::ResetAcceleration() const
{
	CharacterMovementComponent->MaxAcceleration = OriginalAccelerationScale;
}

void UWallRunner::ResetOrientationToMovement() const
{
	CharacterMovementComponent->bOrientRotationToMovement = bOriginalOrientRotationToMovement;
}

FRotator UWallRunner::GetWallOrientation(const FWallHitData& WallHitData) const
{
	if (!IsValid(Owner) || !WallHitData.IsWallHitValid())
		return FRotator::ZeroRotator;
	
	const FVector ImpactNormal = WallHitData.HitResult.ImpactNormal.GetSafeNormal();
	FVector WallForward =
		WallHitData.bIsOnRight
			? FVector::CrossProduct(Owner->GetActorUpVector(), ImpactNormal).GetSafeNormal()
			: FVector::CrossProduct(ImpactNormal, Owner->GetActorUpVector()).GetSafeNormal();

	if (WallForward.IsNearlyZero())
		WallForward = Owner->GetActorForwardVector(); // fallback

	const FRotator WallOrientation = FRotationMatrix::MakeFromXZ(WallForward, Owner->GetActorUpVector()).Rotator();

#if WITH_EDITORONLY_DATA
	if (bShowDebug)
	{
		DrawDebugLine(
			GetWorld(),
			Owner->GetActorLocation(),
			Owner->GetActorLocation() + WallOrientation.Vector() * 100.f,
			FColor::Red,
			false,
			0,
			0,
			2.0f
		);
	}
#endif

	return WallOrientation;
}

void UWallRunner::BeginPlay()
{
	Super::BeginPlay();
	Init();

	if (!Check())
	{
		UE_LOG(LogFlowMotion, Error, TEXT("WallRunner::BeginPlay: Check failed. Disabling component."));
		SetActive(false);
		return;
	}

	CacheMovementComponentData();
	SetupMachine();
}

void UWallRunner::Init()
{
	Owner = GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("WallRunner::Init: Owner is not valid."));
		return;
	}

	CharacterMovementComponent = Owner->GetComponentByClass<UCharacterMovementComponent>();
	if (!IsValid(CharacterMovementComponent))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("WallRunner::Init: CharacterMovementComponent is not valid."));
		return;
	}
}

void UWallRunner::SetupMachine()
{
	UWallRunContext* WallRunContext = UFlowMotionFactory::CreateWallRunContext(this, CharacterMovementComponent);

	MotionMachine = UFlowMotionFactory::CreateMotionMachine(WallRunContext);

	UMotionState* InitState = UFlowMotionFactory::CreateMotionState(
		this,
		UWallRunInitState::StaticClass()
	);

	UMotionState* FallingState = UFlowMotionFactory::CreateMotionState(
		this,
		UWallRunFallingState::StaticClass()
	);

	UMotionState* InputState = UFlowMotionFactory::CreateMotionState(
		this,
		UWallRunInputState::StaticClass()
	);

	UMotionState* WallCheckState = UFlowMotionFactory::CreateMotionState(
		this,
		UWallRunCheckState::StaticClass()
	);

	UMotionState* AttachmentState = UFlowMotionFactory::CreateMotionState(
		this,
		UWallRunAttachmentState::StaticClass()
	);

	UMotionState* WallRunState = UFlowMotionFactory::CreateMotionState(
		this,
		UWallRunningState::StaticClass()
	);

	MotionMachine->AddState(InitStateName, InitState);
	MotionMachine->AddState(FallingStateName, FallingState);
	MotionMachine->AddState(InputStateName, InputState);
	MotionMachine->AddState(CheckStateName, WallCheckState);
	MotionMachine->AddState(AttachStateName, AttachmentState);
	MotionMachine->AddState(RunningStateName, WallRunState);
}

void UWallRunner::CacheMovementComponentData()
{
	OriginalAccelerationScale = CharacterMovementComponent->MaxAcceleration;
	OriginalSpeedScale = CharacterMovementComponent->MaxWalkSpeed;
	bOriginalOrientRotationToMovement = CharacterMovementComponent->bOrientRotationToMovement;
	OriginalGravityScale = CharacterMovementComponent->GravityScale;
}

bool UWallRunner::Check() const
{
	return IsValid(CharacterMovementComponent) && IsValid(Owner);
}
