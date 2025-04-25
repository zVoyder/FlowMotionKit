// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "GrindableRail.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWMOTION_API UGrindableRail : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind")
	bool bIsEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind", meta = (EditCondition = "bOverrideRailOffset"))
	float RailOffsetOverride = 125.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rail Grind", meta = (EditCondition = "bOverrideSpeed"))
	float RailSpeedOverride = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rail Grind", meta = (EditCondition = "bOverrideDetachDistance"))
	float DetachDistanceOverride = 200.f;

private:
	UPROPERTY()
	bool bOverrideRailOffset;
	UPROPERTY()
	bool bOverrideSpeed;
	UPROPERTY()
	bool bOverrideDetachDistance;
	UPROPERTY()
	USplineComponent* SplineComponent;
	
public:
	UGrindableRail();

	/**
	 * @brief Gets the closest distance on the spline to a given world location.
	 * @param WorldLocation The world location to check.
	 * @return The closest distance on the spline to the given world location.
	 */
	UFUNCTION(BlueprintPure)
	float GetClosestDistanceOnSpline(const FVector& WorldLocation) const;

	/**
	 * @brief Gets the spline component associated with this rail.
	 * @return The spline component associated with this rail.
	 */
	UFUNCTION(BlueprintPure)
	USplineComponent* GetSplineComponent() const;

	/**
	 * @brief Checks if the rail has an offset override.
	 * @return True if the rail has an offset override, false otherwise.
	 */
	bool HasRailOffsetOverride() const;

	/**
	 * @brief Checks if the rail has a speed override.
	 * @return True if the rail has a speed override, false otherwise.
	 */
	bool HasRailSpeedOverride() const;

	/**
	 * @brief Checks if the rail has a detach distance override.
	 * @return True if the rail has a detach distance override, false otherwise.
	 */
	bool HasDetachDistanceOverride() const;
	
protected:
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Checks if the rail grinder component is valid and has all required components.
	 * @return True if the rail grinder component is valid, false otherwise.
	 */
	bool Check() const;
};
