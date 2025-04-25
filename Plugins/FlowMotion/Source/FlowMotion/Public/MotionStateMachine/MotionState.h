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
	/**
	 * @brief Initializes the state.
	 */
	void Init();

protected:
	/**
	 * @brief Called when the state is created.
	 */
	virtual void OnInit();

	/**
	 * @brief Called when the state is added to a motion machine.
	 * @param InMachine The motion machine that the state is added to.
	 */
	virtual void OnAdded(UMotionMachine* InMachine);

	/**
	 * @brief Called when the state is removed from a motion machine.
	 * @param InMachine The motion machine that the state is removed from.
	 */
	virtual void OnRemoved(UMotionMachine* InMachine);

	/**
	 * @brief Called when the state is entered.
	 */
	virtual void OnEnter();

	/**
	 * @brief Called when the state is processed.
	 * @param DeltaTime The time since the last frame.
	 */
	virtual void OnProcess(float DeltaTime);

	/**
	 * @brief Called when the state is exited.
	 */
	virtual void OnExit();

	/**
	 * @brief Called when the motion machine is stopped.
	 */
	virtual void OnAbort();

	/**
	 * @brief Tries to transition to a new state.
	 * @param StateName The name of the state to transition to.
	 */
	void TransitionTo(const FName& StateName) const;

	/**
	 * @brief Gets the motion machine that this state belongs to.
	 * @return The motion machine that this state belongs to.
	 */
	UMotionMachine* GetMachine() const;

	/**
	 * @brief Gets the context of the motion machine that this state belongs to.
	 * @return The context of the motion machine that this state belongs to.
	 */
	UMotionContext* GetMachineContext() const;

private:
	/**
	 * @brief Sets the motion machine that this state belongs to.
	 * @param InMachine The motion machine to set.
	 */
	void AddInMachine(UMotionMachine* InMachine);

	/**
	 * @brief Sets the motion machine that this state belongs to to null.
	 * @param InMachine The motion machine that this state belongs to.
	 */
	void RemoveFromMachine(UMotionMachine* InMachine);
};
