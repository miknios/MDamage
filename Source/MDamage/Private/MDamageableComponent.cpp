// Fill out your copyright notice in the Description page of Project Settings.


#include "MDamageableComponent.h"

#include "MDamageableOnDamageEffectSetupAsset.h"
#include "MDamageableOnDamageEffect_Base.h"
#include "MDamageDealEffect_Base.h"
#include "MFloatSource.h"
#include "MGameplayAttribute_Health.h"
#include "MGameplayAttribute_HealthMax.h"
#include "MMultisourceLockableBool.h"
#include "MSimpleGAS/GameplayAttribute/MGameplayAttributeComponent.h"
#include "Team/MTeamLibrary.h"


UMDamageableComponent::UMDamageableComponent()
{
}

void UMDamageableComponent::OnRegister()
{
	Super::OnRegister();
}

void UMDamageableComponent::BeginPlay()
{
	Super::BeginPlay();

	[this]
	{
		const AActor* Owner = GetOwner();
		if (!ensureAlways(IsValid(Owner)))
		{
			return;
		}

		GameplayAttributeComponent = Owner->FindComponentByClass<UMGameplayAttributeComponent>();
		if (!ensureAlways(IsValid(GameplayAttributeComponent)))
		{
			return;
		}

		[this]
		{
			UMGameplayAttribute_Health* HealthAttribute =
				GameplayAttributeComponent->FindAttributeInstanceCasted<UMGameplayAttribute_Health>();

			HealthAttribute->Initialize(HealthMaxInitial);
		}();

		[this]
		{
			UMGameplayAttribute_HealthMax* HealthMaxAttribute =
				GameplayAttributeComponent->FindAttributeInstanceCasted<UMGameplayAttribute_HealthMax>();

			HealthMaxAttribute->Initialize(HealthMaxInitial);
		}();
	}();

	if (IsValid(OnDamageEffectSetupAsset))
	{
		for (UMDamageableOnDamageEffect_Base* OnDamageEffect : OnDamageEffectSetupAsset->OnDamageEffects)
		{
			if (!IsValid(OnDamageEffect))
			{
				continue;
			}

			UMDamageableOnDamageEffect_Base* OnDamageEffectInstanced = NewObject<UMDamageableOnDamageEffect_Base>(
				this, OnDamageEffect->GetClass(), NAME_None, RF_Transient | RF_Public, OnDamageEffect);
			OnDamageEffectInstanced->InitializeOnDamageEffect(this);
			OnDamageEffects.Emplace(OnDamageEffectInstanced);
		}
	}

	InvulnerabilityMultisourceLockableBool = NewObject<UMMultisourceLockableBool>();
	InvulnerabilityMultisourceLockableBool->OnValueChangedDelegate.AddUniqueDynamic(
		this, &UMDamageableComponent::OnInvulnerabilityMultisourceLockableBoolValueChange);
}

#if ENABLE_VISUAL_LOG
void UMDamageableComponent::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
	FVisualLogStatusCategory& CmpCategory = Snapshot->Status.AddZeroed_GetRef();
	CmpCategory.Category = TEXT("M Damageable Component");

	const FString HealthDebugStr = HasInfiniteHealth()
		                               ? TEXT("Infinite")
		                               : FString::Printf(TEXT("%.2f/%.2f"), GetHealthCurrent(), GetHealthMax());
	CmpCategory.Add(TEXT("Health"), HealthDebugStr);

	CmpCategory.Add(TEXT("Invulnerable"), IsInvulnerable() ? TEXT("True") : TEXT("False"));
}
#endif

void UMDamageableComponent::Reset_Implementation(bool bHardReset)
{
	if (bHardReset)
	{
		ResetToMax();
	}
}

UMGameplayAttribute_Health* UMDamageableComponent::GetHealthAttribute() const
{
	if (GameplayAttributeComponent == nullptr)
	{
		return nullptr;
	}

	return GameplayAttributeComponent->FindAttributeInstanceCasted<UMGameplayAttribute_Health>();
}

UMGameplayAttribute_HealthMax* UMDamageableComponent::GetHealthMaxAttribute() const
{
	if (GameplayAttributeComponent == nullptr)
	{
		return nullptr;
	}

	return GameplayAttributeComponent->FindAttributeInstanceCasted<UMGameplayAttribute_HealthMax>();
}

void UMDamageableComponent::DealDamage_Implementation(const FMDamageInstanceData& DamageData)
{
	UMGameplayAttribute_Health* HealthAttribute = GetHealthAttribute();
	const UMGameplayAttribute_HealthMax* HealthMaxAttribute = GetHealthMaxAttribute();

	if (!IsValid(HealthAttribute) || !IsValid(HealthMaxAttribute))
	{
		return;
	}

	const bool bCanReceiveDamage = CanReceiveDamage(DamageData);
	if (!bCanReceiveDamage)
	{
		return;
	}

	const float Damage = [&DamageData]
	{
		if (DamageData.DamageConfig.DamageAmountFloat == nullptr)
		{
			return static_cast<float>(DamageData.DamageConfig.DamageAmount);
		}

		return DamageData.DamageConfig.DamageAmountFloat->GetFloatValue(DamageData.Instigator);
	}();

	const int32 HealthOld = GetHealthCurrent();
	const int32 HealthNew = bInfiniteHealth ? HealthOld : FMath::Max(HealthOld - Damage, 0);
	HealthAttribute->SetBaseValue(HealthNew);

	for (UMDamageDealEffect_Base* DamageDealEffect : DamageData.DamageConfig.DamageDealEffects)
	{
		DamageDealEffect->ApplyDamageDealEffect(this, DamageData);
	}

	for (UMDamageableOnDamageEffect_Base* OnDamageEffect : OnDamageEffects)
	{
		if (OnDamageEffect->CanExecuteOnDamageEffect())
		{
			OnDamageEffect->ApplyOnDamageEffect(DamageData);
		}
	}

	OnDamagedDelegate.Broadcast(FMOnDamagedData(HealthOld, HealthNew, DamageData));

	if (HealthOld != HealthNew && !IsAlive())
	{
		OnDeathDelegate.Broadcast(FMOnDeathData(DamageData));
	}
}

float UMDamageableComponent::GetHealthCurrent() const
{
	const UMGameplayAttribute_Health* HealthAttribute = GetHealthAttribute();
	if (!IsValid(HealthAttribute))
	{
		return FLT_MAX;
	}

	return bInfiniteHealth ? FLT_MAX : HealthAttribute->GetCurrentValue();
}

float UMDamageableComponent::GetHealthMax() const
{
	const UMGameplayAttribute_HealthMax* HealthMaxAttribute = GetHealthMaxAttribute();
	if (!IsValid(HealthMaxAttribute))
	{
		return FLT_MAX;
	}

	return bInfiniteHealth ? FLT_MAX : HealthMaxAttribute->GetCurrentValue();
}

bool UMDamageableComponent::HasInfiniteHealth() const
{
	return bInfiniteHealth;
}

bool UMDamageableComponent::IsAlive() const
{
	return bInfiniteHealth ? true : GetHealthCurrent() > 0;
}

void UMDamageableComponent::Kill(const FVector& DamageDirection, AActor* Instigator)
{
	const FMDamageInstanceData DamageData(FMDamageConfig(GetHealthCurrent()), DamageDirection, Instigator);
	DealDamage(DamageData);
}

void UMDamageableComponent::ResetToMax()
{
	UMGameplayAttribute_Health* HealthAttribute = GetHealthAttribute();
	if (HealthAttribute == nullptr)
	{
		return;
	}

	HealthAttribute->SetBaseValue(GetHealthMax());

	OnHealthResetToMaxDelegate.Broadcast();
}

bool UMDamageableComponent::CanReceiveDamage_Implementation(const FMDamageInstanceData& Damage)
{
	if (IsInvulnerable())
	{
		return false;
	}

	if (!IsAlive())
	{
		return false;
	}

	const bool bAffiliationFilterPassed = UMTeamLibrary::EvaluateAffiliationFilterForActors(Damage.DamageConfig.AffiliationFilter,
	                                                                                        GetOwner(),
	                                                                                        Damage.Instigator);
	return bAffiliationFilterPassed;
}

bool UMDamageableComponent::IsInvulnerable() const
{
	return !InvulnerabilityMultisourceLockableBool->IsActive();
}

void UMDamageableComponent::AddInvulnerabilitySource(const UObject* Source)
{
	InvulnerabilityMultisourceLockableBool->AddBlockSource(Source);
}

void UMDamageableComponent::RemoveInvulnerabilitySource(const UObject* Source)
{
	InvulnerabilityMultisourceLockableBool->RemoveBlockSource(Source);
}

void UMDamageableComponent::OnInvulnerabilityMultisourceLockableBoolValueChange(bool bIsActiveNew)
{
	OnInvulnerabilityStateChangedDelegate.Broadcast(IsInvulnerable());
}
