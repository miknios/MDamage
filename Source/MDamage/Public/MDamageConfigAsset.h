// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Team/MTeamTypes.h"
#include "MDamageConfigAsset.generated.h"

class UMFloatSource_Base;
class UMDamageDealEffect_Base;

USTRUCT(BlueprintType)
struct MDAMAGE_API FMDamageConfig
{
	GENERATED_BODY()

	FMDamageConfig() = default;

	explicit FMDamageConfig(const int32 InDamageAmount)
		: DamageAmount(InDamageAmount)
	{
	}

	FMDamageConfig(const int32 InDamageAmount, const FMTeamAffiliationFilter& InAffiliationFilter)
		: DamageAmount(InDamageAmount),
		  AffiliationFilter(InAffiliationFilter)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 DamageAmount = 0;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMFloatSource_Base> DamageAmountFloat = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMTeamAffiliationFilter AffiliationFilter = FMTeamAffiliationFilter();

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<TObjectPtr<UMDamageDealEffect_Base>> DamageDealEffects;
};

UCLASS(BlueprintType)
class MDAMAGE_API UMDamageConfigAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
	FMDamageConfig DamageConfig;
};
