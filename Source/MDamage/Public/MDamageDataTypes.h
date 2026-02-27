// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MDamageConfigAsset.h"
#include "MDamageDataTypes.generated.h"

extern TAutoConsoleVariable<bool> CVarShowDamageColliders;

USTRUCT(BlueprintType)
struct MDAMAGE_API FMDamageInstanceData
{
	GENERATED_BODY()

	FMDamageInstanceData() = default;

	explicit FMDamageInstanceData(const int32 DamageAmount)
		: DamageConfig(FMDamageConfig(DamageAmount))
	{
	}

	explicit FMDamageInstanceData(const FMDamageConfig& InDamageConfig)
		: DamageConfig(InDamageConfig)
	{
	}

	FMDamageInstanceData(const FMDamageConfig& InDamageConfig, AActor* InInstigator)
		: DamageConfig(InDamageConfig),
		  Instigator(InInstigator)
	{
	}

	FMDamageInstanceData(const FMDamageConfig& InDamageConfig, const FVector& InDamageDirection, AActor* InInstigator)
		: DamageConfig(InDamageConfig),
		  DamageDirection(InDamageDirection),
		  Instigator(InInstigator)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMDamageConfig DamageConfig = FMDamageConfig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DamageDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator = nullptr;
};


USTRUCT(BlueprintType)
struct FMOnDamagedData
{
	GENERATED_BODY()

	FMOnDamagedData() = default;

	FMOnDamagedData(const int32 HealthOld, const int32 HealthNew, const FMDamageInstanceData& DamageInstanceData)
		: HealthOld(HealthOld),
		  HealthNew(HealthNew),
		  DamageInstanceData(DamageInstanceData)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealthOld = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealthNew = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FMDamageInstanceData DamageInstanceData = FMDamageInstanceData();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMOnDamagedSignature, const FMOnDamagedData&, OnDamagedData);

USTRUCT(BlueprintType)
struct FMOnDeathData
{
	GENERATED_BODY()

	FMOnDeathData() = default;

	explicit FMOnDeathData(const FMDamageInstanceData& DamageInstanceData)
		: DamageInstanceData(DamageInstanceData)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FMDamageInstanceData DamageInstanceData = FMDamageInstanceData();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMOnDeathSignature, FMOnDeathData, OnDeathData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMInvulnerabilityStateChangedSignature, bool, InvulnerableStateNew);

USTRUCT(BlueprintType)
struct FMOnHealedData
{
	GENERATED_BODY()

	FMOnHealedData() = default;

	FMOnHealedData(int32 HealthOld, int32 HealthNew, int32 HealAmount, AActor* Instigator)
		: HealthOld(HealthOld),
		  HealthNew(HealthNew),
		  HealAmount(HealAmount),
		  Instigator(Instigator)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealthOld = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealthNew = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HealAmount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Instigator = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMOnHealedSignature, const FMOnHealedData&, OnHealedData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMOnHealthResetToMaxSignature);
