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
	/**
	 * @brief Initializes the motion machine with the given context.
	 * @param InContext The context to initialize the machine with. If null, the machine will have no context.
	 */
	void Init(UMotionContext* InContext = nullptr);

	/**
	 * @brief Starts the motion machine with the specified initial state.
	 * @param InitialStateTag The tag of the initial state to start the machine with.
	 */
	void StartMachine(FName InitialStateTag);

	/**
	 * @brief Stops the motion machine and resets its state.
	 */
	void StopMachine();

	/**
	 * @brief Gets the context associated with the motion machine or null if not set.
	 * @return The context associated with the motion machine.
	 */
	UMotionContext* GetContext() const;

	/**
	 * @brief Checks if the motion machine is currently running.
	 * @return True if the machine is running, false otherwise.
	 */
	bool IsRunning() const;

	/**
	 * @brief Tries to change the current state of the machine to the specified state.
	 * @param StateTag The tag of the state to change to.
	 * @return True if the state change was successful, false otherwise.
	 */
	bool TryChangeState(FName StateTag);

	/**
	 * @brief Adds a new state to the machine.
	 * @param StateTag The tag to identify the state.
	 * @param State The state to add to the machine.
	 */
	void AddState(FName StateTag, UMotionState* State);

	/**
	 * @brief Removes a state from the machine.
	 * @param StateTag The tag of the state to remove.
	 * @return The removed state, or null if the state was not found.
	 */
	UMotionState* RemoveState(FName StateTag);

	/**
	 * @brief Checks if the specified state is currently active.
	 * @param StateTag The tag of the state to check.
	 * @return True if the state is active, false otherwise.
	 */
	bool IsStateActive(FName StateTag) const;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

private:
	/**
	 * @brief Processes the current state of the machine.
	 * @param DeltaTime The time since the last frame.
	 */
	void ProcessCurrentState(float DeltaTime) const;
};
