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

	FRailHitData(): Rail(nullptr)
	{
	}

	FRailHitData(const FRailHitData& Other)
		: HitResult(Other.HitResult),
		  Rail(Other.Rail)
	{
	}

	bool IsRailHitValid() const
	{
		return IsValid(Rail) && HitResult.bBlockingHit;
	}

	bool operator==(const FRailHitData& Other) const
	{
		return Rail == Other.Rail;
	}
};
