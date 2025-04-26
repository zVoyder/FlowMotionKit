// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RailGrinderSystem/RailSpline.h"
#include "RailHitData.generated.h"

USTRUCT(BlueprintType)
struct FLOWMOTION_API FRailHitData
{
	GENERATED_BODY()

	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	UGrindableRail* Rail;
	UPROPERTY()
	bool bIsGoingReverse;

	FRailHitData(): Rail(nullptr),
	                bIsGoingReverse(false)
	{
	}

	FRailHitData(const FHitResult& InHitResult, UGrindableRail* InRail, const bool bInIsGoingReverse = false)
		: HitResult(InHitResult),
		  Rail(InRail),
		  bIsGoingReverse(bInIsGoingReverse)
	{
	}

	FRailHitData(const FRailHitData& Other)
		: HitResult(Other.HitResult),
		  Rail(Other.Rail),
		  bIsGoingReverse(Other.bIsGoingReverse)
	{
	}

	/**
	 * @brief Checks if the rail hit data is valid.
	 * @return True if the rail hit data is valid, false otherwise.
	 */
	bool IsRailHitValid() const
	{
		return IsValid(Rail) && HitResult.bBlockingHit;
	}

	bool operator==(const FRailHitData& Other) const
	{
		return Rail == Other.Rail;
	}
};
