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

	/**
	 * @brief Attempts to attach the character to the rail.
	 */
	UFUNCTION(BlueprintCallable)
	void AttachToRail();

	/**
	 * @brief Attempts to detach the character from the rail.
	 */
	UFUNCTION(BlueprintCallable)
	void DetachFromRail();

	/**
	 * @brief Checks if the character wants to grind.
	 * @return True if the character wants to grind, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool WantsToGrind() const;

	/**
	 * @brief Checks if the character is grinding.
	 * @return True if the character is grinding, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsRailGrinding() const;

	/**
	 * @brief Moves and rotates the character along the rail.
	 * @param DeltaTime Delta time since the last frame.
	 * @param CurrentSplineDistance Current distance along the spline.
	 * @param RailHitData The rail hit data.
	 * @param bIsGoingReverse True if the character is going in reverse, false otherwise.
	 */
	virtual void MoveAndRotateCharacterAlongRail(float DeltaTime, float& CurrentSplineDistance, const FRailHitData& RailHitData, bool& bIsGoingReverse);

	/**
	 * @brief Tries to get the most valid rail hit data.
	 * @param OutRailHitData The rail hit data.
	 * @return True if a valid rail hit was found, false otherwise.
	 */
	virtual bool TryGetMostValidRailHit(FRailHitData& OutRailHitData) const;

	/**
	 * @brief Gets a hits map of grindable rails.
	 * @return A map of grindable rails and their corresponding hit results.
	 */
	TMap<UGrindableRail*, FHitResult> GetRailsHitsMap() const;

	/**
	 * @brief Gets the rail speed for a if provided by the rail or the default speed.
	 * @param Rail The rail to get the speed for.
	 * @return The speed of the rail.
	 */
	float GetRailSpeed(const UGrindableRail* Rail) const;

	/**
	 * @brief Gets the rail offset for a rail if provided by the rail or the default offset.
	 * @param Rail The rail to get the offset for.
	 * @return The offset of the rail.
	 */
	float GetRailOffset(const UGrindableRail* Rail) const;

	/**
	 * @brief Gets the trace location.
	 * @return The trace location.
	 */
	FVector GetTraceLocation() const;

	/**
	 * @brief Resets the movement component data.
	 */
	void ResetMovementComponentData() const;

	/**
	 * @brief Resets the movement mode to its original mode.
	 */
	void ResetMovementMode() const;

	/**
	 * @brief Resets the orientation to movement to its original state.
	 */
	void ResetOrientationToMovement() const;

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Initializes the rail grinder component.
	 */
	void Init();

	/**
	 * @brief Sets up the machine context.
	 */
	void SetupMachine();

	/**
	 * @brief Sets up the machine state.
	 */
	void CacheMovementComponentData();

	/**
	 * @brief Checks if the rail grinder component is valid and has all required components.
	 * @return True if the rail grinder component is valid, false otherwise.
	 */
	bool Check() const;
};
