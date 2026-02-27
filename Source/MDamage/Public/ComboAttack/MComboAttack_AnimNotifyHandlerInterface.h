// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MComboAttack_AnimNotifyHandlerInterface.generated.h"

UINTERFACE()
class UMComboAttack_AnimNotifyHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

class MDAMAGE_API IMComboAttack_AnimNotifyHandlerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void HandleInputGatherForNextAttackEnable();
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleInputGatherForNextAttackDisable();

	UFUNCTION(BlueprintNativeEvent)
	void HandleUnskippableSegmentStarted();

	UFUNCTION(BlueprintNativeEvent)
	void HandleUnskippableSegmentFinished();

	UFUNCTION(BlueprintNativeEvent)
	void HandleMovementResumeSegmentStarted();

	UFUNCTION(BlueprintNativeEvent)
	void HandleMovementResumeSegmentFinished();
};
