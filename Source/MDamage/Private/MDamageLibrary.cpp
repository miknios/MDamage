// Copyright (c) Miknios. All rights reserved.


#include "MDamageLibrary.h"

#include "MDamageableComponent.h"

UMDamageableComponent* UMDamageLibrary::GetDamageableComponent(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UMDamageableComponent>();
}

void UMDamageLibrary::DealDamage(const FMDamageInstanceData& DamageInstanceData, AActor* Target)
{
	if (!IsValid(Target))
	{
		return;
	}

	UMDamageableComponent* DamageableComponent = GetDamageableComponent(Target);
	if (IsValid(DamageableComponent))
	{
		DamageableComponent->DealDamage(DamageInstanceData);
	}
}

bool UMDamageLibrary::IsDamageCollidersDebugEnabled()
{
	return CVarShowDamageColliders.GetValueOnAnyThread();
}
