// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MComboAttack_InputGather_AnimNotify.generated.h"

/**
 * Animation segment during which we gather input to trigger the next attack in combo
 * If input was gathered at any point of the segment, it should trigger the attack as soon as we are out of an unskippable segment
 */
UCLASS(DisplayName = "MCombo - Input Gather")
class MDAMAGE_API UMComboAttack_InputGather_AnimNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// UAnimNotifyState
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
							 const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
						   const FAnimNotifyEventReference& EventReference) override;
	// ~ UAnimNotifyState
};
