﻿// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RunnableWall.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionStateMachine/MotionMachine.h"
#include "WallRunSystem/Data/WallHitData.h"
#include "WallRunner.generated.h"

class UWallRunContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWallRunAttach
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWallRunDetach
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWallRunLaunch
);

const FName WallRunInitStateName = TEXT("WallRunInitState");
const FName WallRunFallingStateName = TEXT("WallRunFallingState");
const FName WallRunInputStateName = TEXT("WallRunInputState");
const FName WallRunAttachmentStateName = TEXT("WallRunAttachmentState");
const FName WallRunCheckStateName = TEXT("WallRunCheckState");
const FName WallRunningStateName = TEXT("WallRunningState");

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWMOTION_API UWallRunner : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWallRunAttach OnWallRunAttach;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWallRunDetach OnWallRunDetach;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWallRunLaunch OnWallRunLaunch;
	
	// === Wall Run: Tracing & Attachment ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WallRun|Tracing")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Tracing")
	float CheckRadius = 75.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Attachment")
	float VelocityToAttach = 450.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Attachment")
	float VelocityToDetach = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Attachment")
	float MinWallRunTime = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Attachment", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float VerticalInertiaConservation = 0.5f;

	// === Wall Run: Physics & Movement ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Movement")
	float DefaultGravityScale = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WallRun|Movement")
	float DefaultMaxSpeedScale = 1.8f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Movement")
	float DefaultAccelerationScale = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Movement")
	float DefaultStickinessStrength = 4.f;

	// === Wall Run: Launch Settings ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Launch")
	float HorizontalLaunchForce = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Launch")
	float VerticalLaunchForce = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Launch", meta = (ToolTip = "Scale applied to the forward launch direction. Increasing this value will make the character launch more forward."))
	float ForwardLaunchScale = 1.2f;

	// === Wall Run: Curve Settings ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Curves")
	bool bUseGravityCurves;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WallRun|Curves", meta = (EditCondition = "bUseGravityCurves", EditConditionHides))
	UCurveFloat* DefaultGravityMultiplierCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WallRun|Curves")
	bool bUseSpeedAccelerationCurves;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WallRun|Curves", meta = (EditCondition = "bUseSpeedAccelerationCurves", EditConditionHides, ToolTip = "Curve used to scale speed (X) and acceleration (Y) during wall run."))
	UCurveVector* DefaultSpeedAccelerationCurve;

	// === Wall Run: Constraints ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Constraints")
	bool bAllowsMultipleAttachOnSameWall = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Constraints", meta = (ClampMin = "0.0", ClampMax = "90.0"))
	float MaxVerticalAngleDifference = 45.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Constraints", meta = (ClampMin = "0.0", ClampMax = "90.0"))
	float MaxHorizontalAngleDifference = 45.f;

	// === Wall Run: Advanced ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Advanced")
	float AttachInputDelay = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Advanced")
	float AttachmentDuration = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Advanced")
	float RotationInterpSpeed = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Advanced")
	FVector TraceOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Advanced")
	float DistanceWeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Advanced")
	float VerticalAngleWeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun|Advanced")
	float HorizontalAngleWeight = 1.0f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "WallRun|Debug")
	bool bShowDebug = false;
#endif

private:
	UPROPERTY()
	UMotionMachine* MotionMachine;
	UPROPERTY()
	UWallRunContext* MachineContext;
	UPROPERTY()
	AActor* Owner;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
	bool bWantsToAttach = false;
	float OriginalGravityScale;
	float OriginalSpeed;
	float OriginalAcceleration;
	bool bOriginalOrientRotationToMovement;
	FVector LaunchDirection;

public:
	UWallRunner();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Attempts to attach the character to the wall.
	 */
	UFUNCTION(BlueprintCallable)
	void AttachToWall();

	/**
	 * @brief Attempts to detach the character from the wall.
	 */
	UFUNCTION(BlueprintCallable)
	void DetachFromWall();

	/**
	 * @brief Checks if the character is currently wall running.
	 * @return True if the character is wall running, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const;

	/**
	 * @brief Checks if the character is wall running on the right side.
	 * @return True if the character is wall running on the right side, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsWallRunningOnRight() const;

	/**
	 * @brief Checks if the character is wall running on the left side.
	 * @return True if the character is wall running on the left side, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsWallRunningOnLeft() const;

	/**
	 * @brief Checks if the character is in the process of attaching to a wall.
	 * @return True if the character is attaching to a wall, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAttachingToWall() const;

	/**
	 * @brief Checks if the character wants to attach to a wall.
	 * @return True if the character wants to attach to a wall, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool WantsToAttach() const;

	/**
	 * @brief Rotates the character along the wall based on the wall's orientation.
	 * @param DeltaTime The time since the last frame.
	 * @param WallOrientation The orientation of the wall.
	 */
	virtual void RotateCharacterAlongWall(float DeltaTime, const FRotator& WallOrientation) const;

	/**
	 * @brief Moves the character along the wall based on the wall's orientation, keeping the character attached to the wall.
	 * @param DeltaTime The time since the last frame.
	 * @param HitResult The hit result of the wall trace.
	 * @param WallOrientation The orientation of the wall.
	 * @param StickinessStrength The strength of the stickiness effect.
	 */
	virtual void MoveCharacterAlongWall(float DeltaTime, const FHitResult& HitResult, const FRotator& WallOrientation, const float StickinessStrength) const;

	/**
	 * @brief Filters the wall hit data to find the most valid wall hit.
	 * @param OutWallHitData The output wall hit data.
	 * @return True if a valid wall hit was found, false otherwise.
	 */
	virtual bool TryGetMostValidWallHit(FWallHitData& OutWallHitData) const;

	/**
	 * @brief Gets a map of wall hit results.
	 * @return A map of wall hit results.
	 */
	TMap<URunnableWall*, FHitResult> GetWallsHitsMap() const;

	/**
	 * @brief Gets the sickiness strength of a wall.
	 * @param Wall The wall to test.
	 * @return The stickiness strength of the wall.
	 */
	float GetStickinessStrength(const URunnableWall* Wall) const;

	/**
	 * @brief Gets the trace location of the actor.
	 * @return The trace location of the actor.
	 */
	FVector GetTraceLocation() const;

	/**
	 * @brief Sets the original gravity scale of the character.
	 * @param GravityScale The original gravity scale to set.
	 */
	void SetOriginalGravityScale(float GravityScale); // In case there are other components that have concurrency with

	/**
	 * @brief Sets the original speed of the character.
	 * @param Speed The original speed scale to set.
	 */
	void SetOriginalSpeed(float Speed);

	/**
	 * @brief Sets the original acceleration of the character.
	 * @param Acceleration The original acceleration scale to set.
	 */
	void SetOriginalAcceleration(float Acceleration);

	/**
	 * @brief Sets the original orientation to movement of the character.
	 * @return True if the original orientation to movement was set, false otherwise.
	 */
	float GetOriginalGravityScale() const;

	/**
	 * @brief Gets the original speed of the character.
	 * @return The original speed of the character.
	 */
	float GetOriginalSpeed() const;

	/**
	 * @brief Gets the original acceleration of the character.
	 * @return The original acceleration of the character.
	 */
	float GetOriginalAcceleration() const;

	/**
	 * @brief Gets the original orientation to movement of the character.
	 * @return The original orientation to movement of the character.
	 */
	bool GetOriginalOrientRotationToMovement() const;

	/**
	 * @brief Resets the movement component data to its original state.
	 */
	void ResetMovementComponentData() const;

	/**
	 * @brief Resets the gravity scale of the character.
	 */
	void ResetGravityScale() const;

	/**
	 * @brief Resets the speed of the character.
	 */
	void ResetSpeed() const;

	/**
	 * @brief Resets the acceleration of the character.
	 */
	void ResetAcceleration() const;

	/**
	 * @brief Resets the orientation to movement of the character.
	 */
	void ResetOrientationToMovement() const;

	/**
	 * @brief Gets the wall orientation based on the wall hit data.
	 * @param WallHitData The wall hit data to use for calculating the orientation.
	 * @return The orientation of the wall.
	 */
	FRotator GetWallOrientation(const FWallHitData& WallHitData) const;

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Initializes the wall runner component.
	 */
	void Init();

	/**
	 * @brief Sets up the motion machine for the wall runner.
	 */
	void SetupMachine();

	/**
	 * @brief Caches the movement component data for the wall runner.
	 */
	void CacheMovementComponentData();

	/**
	 * @brief Checks if the wall runner component is valid and has all required components.
	 * @return True if the wall runner component is valid, false otherwise.
	 */
	bool Check() const;
};
