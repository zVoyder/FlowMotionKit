// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/FlowMotionKitCharacter.h"
#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

UCLASS()
class FLOWMOTIONKIT_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

public:
	virtual void BeginPlay() override;
};
