// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/WallRunner.h"
#include "FlowMotion.h"
#include "Factories/FlowMotionFactory.h"
#include "WallRunSystem/Machine/WallRunContext.h"
#include "WallRunSystem/Machine/States/WallRunFallingState.h"
#include "WallRunSystem/Machine/States/WallRunCheckState.h"
#include "WallRunSystem/Machine/States/WallRunInitState.h"
#include "WallRunSystem/Machine/States/WallRunInputState.h"
#include "WallRunSystem/Machine/States/WallRunningState.h"

UWallRunner::UWallRunner(): PivotOffset(),
                            MotionMachine(nullptr),
                            Owner(nullptr),
                            CharacterMovementComponent(nullptr)
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

bool UWallRunner::IsWallRunning() const
{
	return MotionMachine->IsStateActive(RunningStateName);
}

bool UWallRunner::WantsToAttach() const
{
	return bWantsToAttach;
}

bool UWallRunner::CheckAttachWall(const float MinVelocityLength, const bool bCheckRight, FHitResult& OutHitResult, URunnableWall*& OutWall) const
{
	const UWorld* World = GetWorld();
	FHitResult& HitResult = OutHitResult;

	if (!IsValid(World))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("WallRunner: World is not valid."));
		return false;
	}
	
	if (MinVelocityLength <= MinVelocityToAttach)
		return false;

	const FVector TraceStartPoint = Owner->GetActorLocation() + PivotOffset;
	const FVector Direction = (Owner->GetActorRightVector() * (bCheckRight ? 1.f : -1.f)).GetSafeNormal();
	const FVector TraceEndPoint = TraceStartPoint + CheckDistance * Direction;

#if WITH_EDITORONLY_DATA
	if (bShowDebug)
	{
		DrawDebugLine(
			World,
			TraceStartPoint,
			TraceEndPoint,
			FColor::Red,
			false,
			0,
			0,
			2.0f
		);
	}
#endif

	World->LineTraceSingleByChannel(HitResult, TraceStartPoint, TraceEndPoint, TraceCheckChannel);

	if (!HitResult.bBlockingHit || !HitResult.GetActor())
		return false;

	OutWall = HitResult.GetActor()->FindComponentByClass<URunnableWall>();

	if (!IsValid(OutWall))
		return false;

	const FVector ImpactNormal = HitResult.ImpactNormal.GetSafeNormal();
	const FVector DirFlat = FVector(Direction.X, Direction.Y, 0.f).GetSafeNormal();
	const FVector NormalFlat = FVector(ImpactNormal.X, ImpactNormal.Y, 0.f).GetSafeNormal();

	const float DotHorizontal = FVector::DotProduct(-DirFlat, NormalFlat);
	const float AngleHorizontalDeg = FMath::RadiansToDegrees(FMath::Acos(DotHorizontal));

	const float DotVertical = FVector::DotProduct(ImpactNormal, Owner->GetActorUpVector());
	const float AngleVerticalDeg = FMath::RadiansToDegrees(FMath::Acos(DotVertical));

	if (AngleHorizontalDeg > MaxHorizontalAngle)
		return false;

	if (AngleVerticalDeg < MinVerticalAngle || AngleVerticalDeg > MaxVerticalAngle)
		return false;

	return true;
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

	UMotionState* WallRunState = UFlowMotionFactory::CreateMotionState(
		this,
		UWallRunningState::StaticClass()
	);

	MotionMachine->AddState(InitStateName, InitState);
	MotionMachine->AddState(FallingStateName, FallingState);
	MotionMachine->AddState(InputStateName, InputState);
	MotionMachine->AddState(CheckStateName, WallCheckState);
	MotionMachine->AddState(RunningStateName, WallRunState);
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

	SetupMachine();
}

void UWallRunner::Init()
{
	Owner = GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("WallRunner: Owner is not valid."));
		return;
	}

	CharacterMovementComponent = Owner->GetComponentByClass<UCharacterMovementComponent>();
	if (!IsValid(CharacterMovementComponent))
	{
		UE_LOG(LogFlowMotion, Warning, TEXT("WallRunner: CharacterMovementComponent is not valid."));
		return;
	}
}

bool UWallRunner::Check() const
{
	return IsValid(CharacterMovementComponent) && IsValid(Owner);
}
