// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GrindableRail.h"
#include "Components/ActorComponent.h"
#include "Data/RailHitData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionStateMachine/MotionMachine.h"
#include "RailGrinder.generated.h"

class URailGrindContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRailGrindAttach
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRailGrindDetach
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRailGrindLaunch
);

const FName RailGrindStartingStateName = TEXT("RailGrindStartingState");
const FName RailGrindCheckStateName = TEXT("RailGrindCheckState");
const FName RailGrindGrindingStateName = TEXT("RailGrindGrindingState");

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWMOTION_API URailGrinder : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRailGrindAttach OnRailGrindAttach;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRailGrindDetach OnRailGrindDetach;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRailGrindLaunch OnRailGrindLaunch;
	
	// === Rail Grind: Tracing & Attachment ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RailGrind|Tracing")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Tracing")
	float CheckRadius = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Attachment")
	float DefaultRailOffset = 125.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Attachment")
	float MinSpeedToGrind = 150.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Attachment")
	float DefaultDetachDistance = 200.f;
	
	// === Rail Grind: Movement ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Movement")
	float DefaultRailSpeed = 500.f;
	
	// === Rail Grind: Launch Settings ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Launch")
	float HorizontalLaunchForce = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Launch")
	float VerticalLaunchForce = 1200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Launch", meta = (ToolTip = "Scale applied to the forward launch direction. Increasing this value will make the character launch more forward."))
	float ForwardLaunchScale = 1.0f;
	
	// === Rail Grind: Advanced ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Advanced")
	FVector TraceOffset = FVector(0.f, 0.f, -90.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Advanced")
	float AttachInputDelay = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Advanced")
	float DistanceWeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RailGrind|Advanced")
	float AngleWeight = 1.5f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RailGrind|Debug")
	bool bShowDebug;
#endif

private:
	UPROPERTY()
	AActor* Owner;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
	UPROPERTY()
	UMotionMachine* MotionMachine;
	UPROPERTY()
	URailGrindContext* MachineContext;
	bool bWantsToGrind;
	EMovementMode OriginalMovementMode;
	bool bOriginalOrientRotationToMovement;

public:
	URailGrinder();
	
	UFUNCTION(BlueprintCallable)
	void AttachToRail();

	UFUNCTION(BlueprintCallable)
	void DetachFromRail();

	UFUNCTION(BlueprintPure)
	bool WantsToGrind() const;

	UFUNCTION(BlueprintPure)
	bool IsRailGrinding() const;
	
	virtual void MoveAndRotateCharacterAlongRail(float DeltaTime, float& CurrentSplineDistance, const FRailHitData& RailHitData, bool& bIsGoingReverse);

	virtual bool TryGetMostValidRailHit(FRailHitData& OutRailHitData) const;
	
	TMap<UGrindableRail*, FHitResult> GetRailsHitsMap() const;

	float GetRailSpeed(const UGrindableRail* Rail) const;

	float GetRailOffset(const UGrindableRail* Rail) const;
	
	FVector GetTraceLocation() const;
	
	void ResetMovementComponentData() const;

	void ResetMovementMode() const;
	
	void ResetOrientationToMovement() const;

protected:
	virtual void BeginPlay() override;

private:
	void Init();

	void SetupMachine();
	
	void CacheMovementComponentData();
	
	bool Check() const;
};
