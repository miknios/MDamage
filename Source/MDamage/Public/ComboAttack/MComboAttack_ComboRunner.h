// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MComboAttack_ComboRunner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMComboRunner_OnComboStartedSignature);

USTRUCT(BlueprintType)
struct FMComboRunner_OnComboProgressedData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 AttackIndex = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMComboRunner_OnComboProgressedSignature,
                                            const FMComboRunner_OnComboProgressedData&, OnComboProgressedData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMComboRunner_OnComboFinishedSignature);

/**
 * Object notified by Combo Attack Handler about various things happening to the character that affects the combo
 * 
 * Attacks should be triggered from outside logic by subscribing to delegates or from ComboRunner subclasses (I prefer the first option, because then this class doesn't go too far into details of the attack itself)
 */
UCLASS()
class MDAMAGE_API UMComboAttack_ComboRunner : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(const int32 InComboLength, const bool InbLoopAttackCombo = false);

	UFUNCTION(BlueprintCallable)
	bool IsComboInProgress() const;

	UPROPERTY(BlueprintAssignable)
	FMComboRunner_OnComboStartedSignature OnComboStartedDelegate;

	UPROPERTY(BlueprintAssignable)
	FMComboRunner_OnComboProgressedSignature OnComboProgressedDelegate;

	UPROPERTY(BlueprintAssignable)
	FMComboRunner_OnComboFinishedSignature OnComboFinishedDelegate;

	virtual void OnInputPressed();
	virtual void OnInputGatherForNextAttackEnable();
	virtual void OnInputGatherForNextAttackDisable();
	virtual void OnUnskippableAttackSegmentStarted();
	virtual void OnUnskippableAttackSegmentFinished();
	virtual void OnComboAnimationFinished();

protected:
	virtual void StartNewCombo();
	virtual void ProgressCombo();
	virtual void FinishCombo();

protected:
	// Config data
	UPROPERTY(Transient, VisibleInstanceOnly)
	bool bLoopAttackCombo;

	UPROPERTY(Transient, VisibleInstanceOnly)
	int32 ComboLength;
	// ~ Config data

	// Runtime data
	UPROPERTY(Transient, VisibleInstanceOnly)
	bool bComboInProgress;

	UPROPERTY(Transient, VisibleInstanceOnly)
	bool bInputGatherForNextAttack;

	UPROPERTY(Transient, VisibleInstanceOnly)
	bool bUnskippableAttackSegment;

	UPROPERTY(Transient, VisibleInstanceOnly)
	bool bAwaitsUnskippableAttackSegmentFinishToProgress;

	UPROPERTY(Transient, VisibleInstanceOnly)
	int32 CurrentAttackIndex;
	// ~ Runtime data
};
