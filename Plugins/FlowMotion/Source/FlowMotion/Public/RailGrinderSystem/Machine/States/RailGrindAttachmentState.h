// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RailGrindStateBase.h"
#include "RailGrindAttachmentState.generated.h"

UCLASS()
class FLOWMOTION_API URailGrindAttachmentState : public URailGrindStateBase
{
	GENERATED_BODY()

private:
	float ElapsedTime = 0.f;
	FVector StartLocation;
	FRotator StartRotation;
	FVector AttachPoint;
	FRotator AttachRotation;
	
protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void OnAbort() override;

private:
	/**
	 * @brief Sets the attach point of the character based on the rail hit data.
	 */
	void SetAttachPoint();

	/**
	 * @brief Interpolates the character's position and rotation to the attach point and rotation.
	 */
	void InterpToAttachPointAndRotation() const;

	/**
	 * @brief Checks if the character is attached to the rail.
	 * @return True if the character is attached to the rail, false otherwise.
	 */
	bool IsAttached() const;
};
