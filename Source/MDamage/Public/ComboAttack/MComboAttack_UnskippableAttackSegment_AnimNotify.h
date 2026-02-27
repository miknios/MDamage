// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MComboAttack_UnskippableAttackSegment_AnimNotify.generated.h"

/**
 * Animation segment during which we want to block skipping animation to the next attack in combo
 */
UCLASS(DisplayName = "MCombo - Unskippable Segment")
class MDAMAGE_API UMComboAttack_UnskippableAttackSegment_AnimNotify : public UAnimNotifyState
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
