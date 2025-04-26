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

	FWallHitData(const FHitResult& InHitResult, URunnableWall* InWall, bool bInIsOnRight)
		: HitResult(InHitResult),
		  Wall(InWall),
		  bIsOnRight(bInIsOnRight)
	{
	}
	
	FWallHitData(const FWallHitData& Other)
		: HitResult(Other.HitResult),
		  Wall(Other.Wall),
		  bIsOnRight(Other.bIsOnRight)
	{
	}

	/**
	 * @brief Checks if the wall hit data is valid.
	 * @return True if the wall hit data is valid, false otherwise.
	 */
	bool IsWallHitValid() const
	{
		return IsValid(Wall) && HitResult.bBlockingHit;
	}

	bool operator==(const FWallHitData& Other) const
	{
		return Wall == Other.Wall;
	}
};
