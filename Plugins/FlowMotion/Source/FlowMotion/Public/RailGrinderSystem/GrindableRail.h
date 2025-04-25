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
	
	UFUNCTION(BlueprintPure)
	float GetClosestDistanceOnSpline(const FVector& WorldLocation) const;

	UFUNCTION(BlueprintPure)
	USplineComponent* GetSplineComponent() const;

	bool HasRailOffsetOverride() const;
	
	bool HasRailSpeedOverride() const;

	bool HasDetachDistanceOverride() const;
	
protected:
	virtual void BeginPlay() override;

private:
	bool Check() const;
};
