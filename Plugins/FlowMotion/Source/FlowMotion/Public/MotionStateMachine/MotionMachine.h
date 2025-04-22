// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MotionContext.h"
#include "MotionState.h"
#include "UObject/Object.h"
#include "MotionMachine.generated.h"

UCLASS()
class FLOWMOTION_API UMotionMachine : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FName, UMotionState*> States;
	UPROPERTY()
	UMotionState* CurrentState;
	UPROPERTY()
	UMotionContext* Context;
	bool bIsRunning;

public:
	void Init(UMotionContext* InContext = nullptr);
	
	void StartMachine(FName InitialStateTag);

	void StopMachine();

	UMotionContext* GetContext() const;

	bool IsRunning() const;

	bool TryChangeState(FName StateTag);

	void AddState(FName StateTag, UMotionState* State);

	UMotionState* RemoveState(FName StateTag);

	bool IsStateActive(FName StateTag) const;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

private:
	void ProcessCurrentState(float DeltaTime) const;
};
