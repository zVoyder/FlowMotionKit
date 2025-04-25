// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/RailGrinder.h"
#include "FlowMotion.h"
#include "Factories/FlowMotionFactory.h"
#include "RailGrinderSystem/GrindableRail.h"
#include "RailGrinderSystem/Machine/States/RailGrindCheckState.h"
#include "RailGrinderSystem/Machine/States/RailGrindGrindingState.h"
#include "RailGrinderSystem/Machine/States/RailGrindStartingState.h"

URailGrinder::URailGrinder(): bShowDebug(false),
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

bool URailGrinder::IsRailGrinding() const
{
	if (!IsValid(MotionMachine))
		return false;
	
	return MotionMachine->IsStateActive(RailGrindGrindingStateName);
}

void URailGrinder::MoveAndRotateCharacterAlongRail(float DeltaTime, float& CurrentSplineDistance, const FRailHitData& RailHitData, bool& bIsGoingReverse)
{
	if (!RailHitData.IsRailHitValid() || !Owner)
		return;
	
	const USplineComponent* Spline = RailHitData.Rail->GetSplineComponent();
	if (!IsValid(Spline))
	{
		UE_LOG(LogFlowMotion, Error, TEXT("URailGrinder::MoveAndRotateCharacterAlongRail: Spline is not valid."));
		return;
	}
	
	CurrentSplineDistance += GetRailSpeed(RailHitData.Rail) * GetSmoothedDeltaTime() * (bIsGoingReverse ? -1.f : 1.f);
	const float SplineLength = Spline->GetSplineLength();
	CurrentSplineDistance = FMath::Clamp(CurrentSplineDistance, 0.f, SplineLength);
	
	const FVector NextLocation = Spline->GetLocationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World);
	const FVector Offset = Owner->GetActorUpVector() * GetRailOffset(RailHitData.Rail);
	const FVector TargetLocation = NextLocation + Offset;
	
	FRotator TargetRotation = Spline->GetRotationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World);
	if (bIsGoingReverse)
	{
		TargetRotation.Yaw += 180.f;
		TargetRotation.Pitch *= -1.f;
		TargetRotation.Roll *= -1.f;
	}
	
	Owner->SetActorRotation(TargetRotation);
	Owner->SetActorLocation(TargetLocation, false, nullptr);
}

bool URailGrinder::TryGetMostValidRailHit(FRailHitData& OutRailHitData) const
{
	const TMap<UGrindableRail*, FHitResult> HitsMap = GetRailsHitsMap();

	if (!IsValid(Owner) || HitsMap.IsEmpty())
		return false;

	const FVector PlayerUp = Owner->GetActorUpVector();
	const FVector PlayerLocation = GetTraceLocation();
	float BestScore = MAX_FLT;

	for (const auto& HitPair : HitsMap)
	{
		UGrindableRail* Rail = HitPair.Key;
		const FHitResult& HitResult = HitPair.Value;
		
		const FVector& ImpactNormal = HitResult.ImpactNormal;
		const float Dot = FVector::DotProduct(PlayerUp.GetSafeNormal(), -ImpactNormal.GetSafeNormal());
		const float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f)));
		
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

TMap<UGrindableRail*, FHitResult> URailGrinder::GetRailsHitsMap() const
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

	TMap<UGrindableRail*, FHitResult> HitsMap;
	for (const FHitResult& HitResult : HitResults)
	{
		if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
		{
			UGrindableRail* Rail = HitResult.GetActor()->FindComponentByClass<UGrindableRail>();
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

float URailGrinder::GetRailSpeed(const UGrindableRail* Rail) const
{
	if (!IsValid(Rail) || !Rail->bIsEnabled || !Rail->HasRailSpeedOverride())
		return DefaultRailSpeed;
	
	return Rail->RailSpeedOverride;
}

float URailGrinder::GetRailOffset(const UGrindableRail* Rail) const
{
	if (!IsValid(Rail) || !Rail->bIsEnabled || !Rail->HasRailOffsetOverride())
		return DefaultRailOffset;
	
	return Rail->RailOffsetOverride;
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

float URailGrinder::GetSmoothedDeltaTime()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	if (SmoothedDeltaTime < 0.f)
		SmoothedDeltaTime = DeltaTime;
	else
		SmoothedDeltaTime = FMath::FInterpTo(SmoothedDeltaTime, DeltaTime, DeltaTime, 5.f);

	return SmoothedDeltaTime;
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
	
	UMotionState* GrindingState = UFlowMotionFactory::CreateMotionState(
		this,
		URailGrindGrindingState::StaticClass()
	);

	MotionMachine->AddState(RailGrindStartingStateName, StartingState);
	MotionMachine->AddState(RailGrindCheckStateName, CheckState);
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
