// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MotionStateMachine/MotionMachine.h"
#include "MachineTesting.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWMOTION_API UMachineTesting : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMotionMachine* Machine;
	
public:
	UMachineTesting();

	UFUNCTION(BlueprintCallable)
	void SetState1() const;

	UFUNCTION(BlueprintCallable)
	void SetState2() const;

	UFUNCTION(BlueprintCallable)
	void StartMachine() const;
		
	UFUNCTION(BlueprintCallable)
	void StopMachine() const;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
