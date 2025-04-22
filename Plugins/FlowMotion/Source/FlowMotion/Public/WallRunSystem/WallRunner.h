// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RunnableWall.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionStateMachine/MotionMachine.h"
#include "WallRunner.generated.h"

const FName InitStateName = TEXT("WallRunInitState");
const FName FallingStateName = TEXT("WallRunFallingState");
const FName InputStateName = TEXT("WallRunInputState");
const FName CheckStateName = TEXT("WallRunCheckState");
const FName RunningStateName = TEXT("WallRunningState");

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWMOTION_API UWallRunner : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun")
	TEnumAsByte<ECollisionChannel> TraceCheckChannel = ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun")
	float CheckDistance = 75.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WallRun")
	UCurveFloat* DefaultGravityMultiplierCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun")
	float DesiredDistanceToWall = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun")
	float TimeBeforeAttach = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun", meta = (ClampMin = "0.0"))
	float MinVelocityToAttach = 150.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun", meta = (ClampMin = "0.0", ClampMax = "360.0"))
	float MaxVerticalAngle = 90.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun", meta = (ClampMin = "0.0", ClampMax = "360.0"))
	float MinVerticalAngle = 85.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun", meta = (ClampMin = "0.0", ClampMax = "360.0"))
	float MaxHorizontalAngle = 45.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WallRun")
	FVector PivotOffset;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShowDebug = false;
#endif

private:
	UPROPERTY()
	UMotionMachine* MotionMachine;
	UPROPERTY()
	AActor* Owner;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
	bool bWantsToAttach = false;

public:
	UWallRunner();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AttachToWall();

	UFUNCTION(BlueprintCallable)
	void DetachFromWall();

	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const;

	UFUNCTION(BlueprintPure)
	bool WantsToAttach() const;
	
	bool CheckAttachWall(float MinVelocityLength, bool bCheckRight, FHitResult& OutHitResult, URunnableWall*& OutWall) const;

protected:
	virtual void BeginPlay() override;

private:
	void Init();

	void SetupMachine();

	bool Check() const;
};
