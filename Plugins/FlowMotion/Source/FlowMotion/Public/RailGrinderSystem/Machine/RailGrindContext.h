// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionStateMachine/MotionContext.h"
#include "RailGrinderSystem/RailGrinder.h"
#include "RailGrindContext.generated.h"

UCLASS()
class FLOWMOTION_API URailGrindContext : public UMotionContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* Owner;
	UPROPERTY()
	URailGrinder* RailGrinder;
	UPROPERTY()
	FRailHitData HitData;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	UPROPERTY()
	bool bIsGoingReverse;
};
