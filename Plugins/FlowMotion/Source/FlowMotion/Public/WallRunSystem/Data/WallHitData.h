// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunSystem/RunnableWall.h"
#include "WallHitData.generated.h"

USTRUCT(BlueprintType)
struct FLOWMOTION_API FWallHitData
{
	GENERATED_BODY()

	UPROPERTY()
	FHitResult HitResult;

	UPROPERTY()
	URunnableWall* Wall;

	UPROPERTY()
	bool bIsOnRight;

	FWallHitData(): Wall(nullptr),
	                bIsOnRight(false)
	{
	}
	
	FWallHitData(const FWallHitData& Other)
		: HitResult(Other.HitResult),
		  Wall(Other.Wall),
		  bIsOnRight(Other.bIsOnRight)
	{
	}

	bool IsWallHitValid() const
	{
		return IsValid(Wall) && HitResult.bBlockingHit;
	}

	bool operator==(const FWallHitData& Other) const
	{
		return Wall == Other.Wall;
	}
};
