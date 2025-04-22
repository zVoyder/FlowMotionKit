// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionStateMachine/MotionContext.h"
#include "WallRunSystem/WallRunner.h"
#include "WallRunContext.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunContext : public UMotionContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* Owner;
	UPROPERTY()
	UWallRunner* Runner;
	UPROPERTY()
	URunnableWall* LastWall;
	UPROPERTY()
	bool bLastWallIsRight;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	FVector StartUpHorVelocity;
};
