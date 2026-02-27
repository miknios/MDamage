// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MDamageableOnDamageEffectSetupAsset.generated.h"

class UMDamageableOnDamageEffect_Base;

/**
 * Used by MDamageableComponent to apply On Damage effects
 */
UCLASS()
class MDAMAGE_API UMDamageableOnDamageEffectSetupAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(Instanced, EditAnywhere, meta = (ShowOnlyInnerProperties))
	TArray<TObjectPtr<UMDamageableOnDamageEffect_Base>> OnDamageEffects;
};
