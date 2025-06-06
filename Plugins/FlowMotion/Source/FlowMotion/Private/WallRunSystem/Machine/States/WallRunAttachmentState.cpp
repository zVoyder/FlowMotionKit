﻿// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunAttachmentState.h"

void UWallRunAttachmentState::OnEnter()
{
	Super::OnEnter();
	ElapsedTime = 0.f;

	const UWallRunContext* Context = GetWallRunContext();
	const UWallRunner* Runner = Context->Runner;
	Context->MovementComponent->GravityScale = 0.f;
	Context->MovementComponent->bOrientRotationToMovement = false;
	ScaleVerticalVelocity(Runner->VerticalInertiaConservation);
	Runner->OnWallRunAttach.Broadcast();
}

void UWallRunAttachmentState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	ElapsedTime += DeltaTime;

	const UWallRunner* Runner = GetWallRunContext()->Runner;
	UWallRunContext* Context = GetWallRunContext();

	if (!Runner->WantsToAttach() || !Runner->TryGetMostValidWallHit(Context->HitData))
	{
		Detach();
		return;
	}
	
	const FRotator WallOrientation = Runner->GetWallOrientation(Context->HitData);
	Runner->RotateCharacterAlongWall(DeltaTime, WallOrientation);
	const float Stickiness = Runner->GetStickinessStrength(Context->HitData.Wall);
	Runner->MoveCharacterAlongWall(DeltaTime, Context->HitData.HitResult, WallOrientation, Stickiness);
	
	if (ElapsedTime >= Runner->AttachmentDuration)
		TransitionTo(WallRunningStateName);
}

void UWallRunAttachmentState::OnAbort()
{
	Super::OnAbort();
	GetWallRunContext()->Runner->ResetMovementComponentData(); // Safe call
}

void UWallRunAttachmentState::Detach() const
{
	GetWallRunContext()->Runner->OnWallRunDetach.Broadcast();
	TransitionTo(WallRunFallingStateName);
}

void UWallRunAttachmentState::ScaleVerticalVelocity(const float Scale) const
{
	const float ZVelocity = GetWallRunContext()->MovementComponent->Velocity.Z * Scale;
	GetWallRunContext()->MovementComponent->Velocity = FVector(GetWallRunContext()->MovementComponent->Velocity.X, GetWallRunContext()->MovementComponent->Velocity.Y, ZVelocity);
}
