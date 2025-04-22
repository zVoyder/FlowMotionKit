// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MotionContext.h"
#include "UObject/Object.h"
#include "MotionState.generated.h"

UCLASS()
class FLOWMOTION_API UMotionState : public UObject
{
	GENERATED_BODY()

	friend class UMotionMachine;

private:
	UPROPERTY()
	UMotionMachine* Machine;

public:
	void Init();

protected:
	virtual void OnInit();

	virtual void OnAdded(UMotionMachine* InMachine);

	virtual void OnRemoved(UMotionMachine* InMachine);

	virtual void OnEnter();

	virtual void OnProcess(float DeltaTime);

	virtual void OnExit();

	virtual void OnAbort();

	void TransitionTo(const FName& StateName) const;

	UMotionMachine* GetMachine() const;

	UMotionContext* GetMachineContext() const;

private:
	void AddInMachine(UMotionMachine* InMachine);

	void RemoveFromMachine(UMotionMachine* InMachine);
};
