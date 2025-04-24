// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RailHitData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionStateMachine/MotionMachine.h"
#include "RailGrinderSystem/RailSpline.h"
#include "RailGrinder.generated.h"

class URailGrindContext;

const FName RailGrindStartingStateName = TEXT("RailGrindStartingState");
const FName RailGrindCheckStateName = TEXT("RailGrindCheckState");
const FName RaildGrindAttachmentStateName = TEXT("RailGrindAttachmentState");
const FName RailGrindGrindingStateName = TEXT("RailGrindGrindingState");

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWMOTION_API URailGrinder : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rail Grind")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	float MinSpeedToGrind = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	float AttachmentDuration = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	float RailSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	float AttachInputDelay = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	float RailOffset = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	float CheckRadius = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	float MaxGrindableAngle = 65.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind")
	FVector TraceOffset = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind|Advanced")
	float DistanceWeight = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind|Advanced")
	float AngleWeight = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind|Advanced")
	float PositionLerpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail Grind|Advanced")
	float RotationLerpSpeed;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug")
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
	
	virtual void MoveAndRotateCharacterAlongRail(float DeltaTime, float& CurrentSplineDistance, const FRailHitData& RailHitData, bool& bIsGoingReverse);

	virtual bool TryGetMostValidRailHit(FRailHitData& OutRailHitData) const;

	TMap<ARailSpline*, FHitResult> GetRailsHitsMap() const;

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
