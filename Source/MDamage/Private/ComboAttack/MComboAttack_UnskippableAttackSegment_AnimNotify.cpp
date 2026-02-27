// Copyright (c) Miknios. All rights reserved.


#include "ComboAttack/MComboAttack_UnskippableAttackSegment_AnimNotify.h"

#include "ComboAttack/MComboAttack_AnimNotifyHandlerInterface.h"

void UMComboAttack_UnskippableAttackSegment_AnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                                    float TotalDuration,
                                                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp))
	{
		return;
	}

	bool bNotify = true;

#if WITH_EDITOR
	bNotify = GEditor->IsPlaySessionInProgress();
#endif

	if (bNotify)
	{
		AActor* MeshOwner = MeshComp->GetOwner();
		if (!IsValid(MeshOwner))
		{
			return;
		}

		TArray<UActorComponent*> HandlerComponents = MeshOwner->GetComponentsByInterface(
			UMComboAttack_AnimNotifyHandlerInterface::StaticClass());
		for (UActorComponent* HandlerComponent : HandlerComponents)
		{
			if (!IsValid(HandlerComponent))
			{
				continue;
			}

			IMComboAttack_AnimNotifyHandlerInterface::Execute_HandleUnskippableSegmentStarted(HandlerComponent);
		}
	}
}

void UMComboAttack_UnskippableAttackSegment_AnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
	{
		return;
	}

	bool bNotify = true;

#if WITH_EDITOR
	bNotify = GEditor->IsPlaySessionInProgress();
#endif

	if (bNotify)
	{
		AActor* MeshOwner = MeshComp->GetOwner();
		if (!IsValid(MeshOwner))
		{
			return;
		}

		TArray<UActorComponent*> HandlerComponents = MeshOwner->GetComponentsByInterface(
			UMComboAttack_AnimNotifyHandlerInterface::StaticClass());
		for (UActorComponent* HandlerComponent : HandlerComponents)
		{
			if (!IsValid(HandlerComponent))
			{
				continue;
			}

			IMComboAttack_AnimNotifyHandlerInterface::Execute_HandleUnskippableSegmentFinished(HandlerComponent);
		}
	}
}
