// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunningState.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunningState : public UWallRunStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	UPROPERTY()
	UCurveFloat* GravityCurve;
	float OriginalGravityScale;
	bool bOriginalOrientRotationToMovement;
	float ElapsedTime = 0.f;
	
protected:
	virtual void OnAdded(UMotionMachine* InMachine) override;

	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void OnAbort() override;

private:
	void RotateCharacterAlongWall(const float DeltaTime, const FRotator& WallOrientation) const;
	
	void MoveCharacterAlongWall(const float DeltaTime, const FHitResult& HitResult, const FRotator& WallOrientation) const;

	void ScaleGravityWithCurve() const;
	
	void SetGravityCurve();
	
	FRotator GetWallOrientation(const bool bIsRight, const FHitResult& HitResult) const;

	void ResetMovementComponent() const;
};
