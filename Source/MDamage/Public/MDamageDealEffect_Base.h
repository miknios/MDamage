// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MDamageDealEffect_Base.generated.h"

struct FMDamageInstanceData;

/**
 * Base class for implementing effects applied along dealing damage like pushing target, adding elemental damage to it etc.
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class MDAMAGE_API UMDamageDealEffect_Base : public UObject
{
	GENERATED_BODY()

	friend class UMDamageableComponent;

public:
	// UObject
	virtual UWorld* GetWorld() const override;
	
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
	// ~ UObject

protected:
	UFUNCTION(BlueprintNativeEvent)
	void ApplyDamageDealEffect(UMDamageableComponent* DamagedComponent, const FMDamageInstanceData& DamageData);
};
