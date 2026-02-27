// Copyright (c) Miknios. All rights reserved.


#include "MDamageDealEffect_Base.h"

#if WITH_EDITOR
bool UMDamageDealEffect_Base::ImplementsGetWorld() const
{
	return true;
}
#endif


UWorld* UMDamageDealEffect_Base::GetWorld() const
{
	return UObject::GetWorld();
}

void UMDamageDealEffect_Base::ApplyDamageDealEffect_Implementation(UMDamageableComponent* DamagedComponent,
                                                                   const FMDamageInstanceData& DamageData)
{
}
