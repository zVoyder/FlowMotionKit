// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/RailGrinder.h"
#include "FlowMotion.h"
#include "Factories/FlowMotionFactory.h"
#include "RailGrinderSystem/Machine/States/RailGrindAttachmentState.h"
#include "RailGrinderSystem/Machine/States/RailGrindCheckState.h"
#include "RailGrinderSystem/Machine/States/RailGrindGrindingState.h"
#include "RailGrinderSystem/Machine/States/RailGrindStartingState.h"

URailGrinder::URailGrinder(): PositionLerpSpeed(0),
                              RotationLerpSpeed(0),
                              bShowDebug(false),
                              Owner(nullptr),
                              CharacterMovementComponent(nullptr),
                              MotionMachine(nullptr),
                              MachineContext(nullptr),
                              bWantsToGrind(false),
                              OriginalMovementMode(),
                              bOriginalOrientRotationToMovement(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URailGrinder::AttachToRail()
{
	bWantsToGrind = true;

	if (!IsValid(MotionMachine))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("URailGrinder::AttachToRail: Motion machine is not valid."));
		return;
	}

	MotionMachine->StartMachine(RailGrindStartingStateName);
}

void URailGrinder::DetachFromRail()
{
	bWantsToGrind = false;

	if (!IsValid(MotionMachine))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("URailGrinder::DetachFromRail: Motion machine is not valid."));
		return;
	}

	MotionMachine->StopMachine();
}

bool URailGrinder::WantsToGrind() const
{
	return bWantsToGrind;
}

void URailGrinder::MoveAndRotateCharacterAlongRail(float DeltaTime, float& CurrentSplineDistance, const FRailHitData& RailHitData, bool& bIsGoingReverse)
{
	if (!RailHitData.IsRailHitValid() || !Owner)
		return;
	
	const USplineComponent* Spline = RailHitData.Rail->GetRailSplineComponent();
	if (!Spline)
		return;
	
	// CurrentSplineDistance += RailSpeed * DeltaTime * (bIsGoingReverse ? -1.f : 1.f);
	// const float SplineLength = Spline->GetSplineLength();
	// CurrentSplineDistance = FMath::Clamp(CurrentSplineDistance, 0.f, SplineLength);
	//
	// const FVector Offset = FVector(0.f, 0.f, RailOffset);
	// const FVector CurrentLocation = Owner->GetActorLocation() + Offset;
	// const FVector TargetLocation = Spline->GetLocationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World) + Offset;
	//
	// Owner->SetActorLocation(TargetLocation);
	//

	
	// FRotator Rotation = Spline->GetRotationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World);
	// // if (bIsGoingReverse)
	// // {
	// // 	Rotation = Rotation + FRotator(0.f, 180.f, 0.f);
	// // 	Rotation.Pitch *= -1.f;
	// //
	// // }
	// Owner->SetActorRotation(Rotation);
	
	const FVector Tangent = Spline->GetTangentAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World);
	CharacterMovementComponent->AddInputVector(Tangent * RailSpeed);
	
	const FVector ClosestPoint = Spline->FindLocationClosestToWorldLocation(Owner->GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector TargetPosition = ClosestPoint + FVector(0, 0, RailOffset);
	const FVector PullDirection = (TargetPosition - Owner->GetActorLocation());
	CharacterMovementComponent->AddImpulse(PullDirection * 4.f, true);
}

bool URailGrinder::TryGetMostValidRailHit(FRailHitData& OutRailHitData) const
{
	const TMap<ARailSpline*, FHitResult> HitsMap = GetRailsHitsMap();

	if (!IsValid(Owner) || HitsMap.IsEmpty())
		return false;

	const FVector PlayerUp = Owner->GetActorUpVector();
	const FVector PlayerLocation = GetTraceLocation();
	float BestScore = MAX_FLT;

	for (const auto& HitPair : HitsMap)
	{
		ARailSpline* Rail = HitPair.Key;
		const FHitResult& HitResult = HitPair.Value;
		
		const FVector& ImpactNormal = HitResult.ImpactNormal;
		const float Dot = FVector::DotProduct(PlayerUp.GetSafeNormal(), -ImpactNormal.GetSafeNormal());
		const float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f)));
		
		// if (AngleDegrees > MaxGrindableAngle)
		// 	continue;

		const float Distance = FVector::Dist(PlayerLocation, HitResult.ImpactPoint);
		const float Score = Distance * DistanceWeight + AngleDegrees * AngleWeight; // Score: lower is better

		if (Score < BestScore)
		{
			BestScore = Score;
			OutRailHitData.HitResult = HitResult;
			OutRailHitData.Rail = Rail;
		}
	}

	if (!OutRailHitData.IsRailHitValid())
		return false;

#if WITH_EDITORONLY_DATA
	if (bShowDebug)
	{
		DrawDebugBox(
			GetWorld(),
			OutRailHitData.HitResult.ImpactPoint,
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

TMap<ARailSpline*, FHitResult> URailGrinder::GetRailsHitsMap() const
{
	if (!Check())
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("URailGrinder::GetRailsHitsMap: Check failed."));
		return {};
	}

	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("URailGrinder::GetRailsHitsMap: World is not valid."));
		return {};
	}

	TArray<FHitResult> HitResults;
	World->SweepMultiByChannel(
		HitResults,
		GetTraceLocation(),
		GetTraceLocation(),
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(CheckRadius)
	);

#if WITH_EDITORONLY_DATA
	if (bShowDebug)
	{
		DrawDebugSphere(
			World,
			GetTraceLocation(),
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

	TMap<ARailSpline*, FHitResult> HitsMap;
	for (const FHitResult& HitResult : HitResults)
	{
		if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
		{
			ARailSpline* Rail = HitResult.GetActor()->IsA(ARailSpline::StaticClass()) ? Cast<ARailSpline>(HitResult.GetActor()) : nullptr;
			if (!IsValid(Rail) || !Rail->bIsEnabled)
				continue;

			HitsMap.Add(Rail, HitResult);

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

FVector URailGrinder::GetTraceLocation() const
{
	if (!IsValid(Owner))
		return FVector::ZeroVector;

	return Owner->GetActorLocation() + TraceOffset;
}

void URailGrinder::ResetMovementComponentData() const
{
	ResetMovementMode();
	ResetOrientationToMovement();
}

void URailGrinder::ResetMovementMode() const
{
	CharacterMovementComponent->SetMovementMode(OriginalMovementMode);
}

void URailGrinder::ResetOrientationToMovement() const
{
	CharacterMovementComponent->bOrientRotationToMovement = bOriginalOrientRotationToMovement;
}

void URailGrinder::BeginPlay()
{
	Super::BeginPlay();
	Init();

	if (!Check())
	{
		UE_LOG(LogFlowMotion, Error, TEXT("URailGrinder::BeginPlay: Check failed. Disabling component."));
		SetActive(false);
		return;
	}

	CacheMovementComponentData();
	SetupMachine();
}

void URailGrinder::Init()
{
	Owner = GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("URailGrinder::Init: Owner is not valid."));
		return;
	}

	CharacterMovementComponent = Owner->GetComponentByClass<UCharacterMovementComponent>();
	if (!IsValid(CharacterMovementComponent))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("URailGrinder::Init: CharacterMovementComponent is not valid."));
		return;
	}
}

void URailGrinder::SetupMachine()
{
	MachineContext = UFlowMotionFactory::CreateRailGrindContext(this, CharacterMovementComponent);
	MotionMachine = UFlowMotionFactory::CreateMotionMachine(MachineContext);

	UMotionState* StartingState = UFlowMotionFactory::CreateMotionState(
		this,
		URailGrindStartingState::StaticClass()
	);

	UMotionState* CheckState = UFlowMotionFactory::CreateMotionState(
		this,
		URailGrindCheckState::StaticClass()
	);

	UMotionState* AttachmentState = UFlowMotionFactory::CreateMotionState(
		this,
		URailGrindAttachmentState::StaticClass()
	);

	UMotionState* GrindingState = UFlowMotionFactory::CreateMotionState(
		this,
		URailGrindGrindingState::StaticClass()
	);

	MotionMachine->AddState(RailGrindStartingStateName, StartingState);
	MotionMachine->AddState(RailGrindCheckStateName, CheckState);
	MotionMachine->AddState(RaildGrindAttachmentStateName, AttachmentState);
	MotionMachine->AddState(RailGrindGrindingStateName, GrindingState);
}

void URailGrinder::CacheMovementComponentData()
{
	OriginalMovementMode = CharacterMovementComponent->MovementMode;
	bOriginalOrientRotationToMovement = CharacterMovementComponent->bOrientRotationToMovement;
}

bool URailGrinder::Check() const
{
	return IsValid(CharacterMovementComponent) && IsValid(Owner);
}
