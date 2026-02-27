// Copyright (c) Miknios. All rights reserved.


#include "MDamageableOnDamageEffect_Base.h"

#include "MDamageableComponent.h"

#if WITH_EDITOR
bool UMDamageableOnDamageEffect_Base::ImplementsGetWorld() const
{
	return true;
}
#endif

bool UMDamageableOnDamageEffect_Base::CanExecuteOnDamageEffect() const
{
	if (!IsValid(DamageableComponentOwner))
	{
		return false;
	}
	
	if (ExecutionFilter == EMDamageableOnDamageEffect_ExecutionFilterType::PlayAlways)
	{
		return true;
	}

	const bool bResult =
		DamageableComponentOwner->IsAlive() && ExecutionFilter == EMDamageableOnDamageEffect_ExecutionFilterType::PlayOnNonDeathHitOnly ||
		!DamageableComponentOwner->IsAlive() && ExecutionFilter == EMDamageableOnDamageEffect_ExecutionFilterType::PlayOnDeathHitOnly;
	return bResult;
}

UWorld* UMDamageableOnDamageEffect_Base::GetWorld() const
{
	return UObject::GetWorld();
}

void UMDamageableOnDamageEffect_Base::InitializeOnDamageEffect_Implementation(UMDamageableComponent* InDamageableComponentOwner)
{
	DamageableComponentOwner = InDamageableComponentOwner;
}

void UMDamageableOnDamageEffect_Base::ApplyOnDamageEffect_Implementation(const FMDamageInstanceData& DamageData)
{
}
