// Copyright (c) Miknios. All rights reserved.


#include "ComboAttack/MComboAttack_ComboRunner.h"

void UMComboAttack_ComboRunner::OnInputPressed()
{
	if (!bComboInProgress)
	{
		StartNewCombo();
	}
	else
	{
		if (bUnskippableAttackSegment)
		{
			if (bInputGatherForNextAttack)
			{
				bAwaitsUnskippableAttackSegmentFinishToProgress = true;
			}
		}
		else
		{
			ProgressCombo();
		}
	}
}

void UMComboAttack_ComboRunner::OnInputGatherForNextAttackEnable()
{
	bInputGatherForNextAttack = true;
}

void UMComboAttack_ComboRunner::OnInputGatherForNextAttackDisable()
{
	bInputGatherForNextAttack = false;
}

void UMComboAttack_ComboRunner::OnUnskippableAttackSegmentStarted()
{
	bUnskippableAttackSegment = true;
}

void UMComboAttack_ComboRunner::OnUnskippableAttackSegmentFinished()
{
	bUnskippableAttackSegment = false;

	if (bAwaitsUnskippableAttackSegmentFinishToProgress)
	{
		ProgressCombo();
	}
}

void UMComboAttack_ComboRunner::OnComboAnimationFinished()
{
	FinishCombo();
}

void UMComboAttack_ComboRunner::Init(const int32 InComboLength, const bool InbLoopAttackCombo)
{
	ComboLength = InComboLength;
	bLoopAttackCombo = InbLoopAttackCombo;
}

bool UMComboAttack_ComboRunner::IsComboInProgress() const
{
	return bComboInProgress;
}

void UMComboAttack_ComboRunner::StartNewCombo()
{
	bComboInProgress = true;
	bInputGatherForNextAttack = false;
	bUnskippableAttackSegment = false;
	bAwaitsUnskippableAttackSegmentFinishToProgress = false;
	CurrentAttackIndex = 0;

	OnComboStartedDelegate.Broadcast();
}

void UMComboAttack_ComboRunner::ProgressCombo()
{
	bAwaitsUnskippableAttackSegmentFinishToProgress = false;
	
	CurrentAttackIndex++;

	if (bLoopAttackCombo)
		CurrentAttackIndex %= ComboLength;

	if (CurrentAttackIndex < ComboLength)
	{
		OnComboProgressedDelegate.Broadcast(FMComboRunner_OnComboProgressedData{CurrentAttackIndex});
	}
	else
	{
		FinishCombo();
	}
}

void UMComboAttack_ComboRunner::FinishCombo()
{
	bComboInProgress = false;

	OnComboFinishedDelegate.Broadcast();
}
