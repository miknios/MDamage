// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MDamageableOnDamageEffect_Base.generated.h"

struct FMDamageInstanceData;

UENUM()
enum class EMDamageableOnDamageEffect_ExecutionFilterType
{
	PlayAlways,
	PlayOnNonDeathHitOnly,
	PlayOnDeathHitOnly,
};

/*
 * Base class for implementing effects applied on UMDamageableComponent owner like Blink, impact SFX etc.
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class MDAMAGE_API UMDamageableOnDamageEffect_Base : public UObject
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

	bool CanExecuteOnDamageEffect() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeOnDamageEffect(UMDamageableComponent* InDamageableComponentOwner);

	UFUNCTION(BlueprintNativeEvent)
	void ApplyOnDamageEffect(const FMDamageInstanceData& DamageData);

	UPROPERTY(EditAnywhere)
	EMDamageableOnDamageEffect_ExecutionFilterType ExecutionFilter = EMDamageableOnDamageEffect_ExecutionFilterType::PlayAlways;

	UPROPERTY
	(Transient, BlueprintReadOnly)
	UMDamageableComponent* DamageableComponentOwner;
};
