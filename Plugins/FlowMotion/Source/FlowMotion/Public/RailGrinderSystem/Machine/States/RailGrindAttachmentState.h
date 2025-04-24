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
	
public:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void OnAbort() override;
	
private:
	void SetDirection() const;
	
	void AttachCharacterOnRail() const;
};
