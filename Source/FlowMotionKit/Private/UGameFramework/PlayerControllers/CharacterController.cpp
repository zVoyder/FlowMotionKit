// Copyright VUEDK, Inc. All Rights Reserved.

#include "UGameFramework/PlayerControllers/CharacterController.h"
#include "EnhancedInputSubsystems.h"

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
}
