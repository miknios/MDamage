// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MResettable.h"
#include "MDamageDataTypes.h"
#include "Components/ActorComponent.h"
#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"
#include "MDamageableComponent.generated.h"

class UMGameplayAttribute_HealthMax;
class UMGameplayAttribute_Health;
class UMGameplayAttributeComponent;
class UMMultisourceLockableBool;
class UMDamageableOnDamageEffectSetupAsset;
class UMDamageableOnDamageEffect_Base;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MDAMAGE_API UMDamageableComponent : public UActorComponent,
                                          public IVisualLoggerDebugSnapshotInterface,
                                          public IMResettable
{
	GENERATED_BODY()

public:
	UMDamageableComponent();

	// UActorComponent
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	// ~ UActorComponent

	// IVisualLoggerDebugSnapshotInterface
#if ENABLE_VISUAL_LOG
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
#endif
	// ~ IVisualLoggerDebugSnapshotInterface

	// IMResettable
	virtual void Reset_Implementation(bool bHardReset) override;
	// ~ IMResettable

	UFUNCTION(BlueprintCallable)
	UMGameplayAttribute_Health* GetHealthAttribute() const;

	UFUNCTION(BlueprintCallable)
	UMGameplayAttribute_HealthMax* GetHealthMaxAttribute() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DealDamage(const FMDamageInstanceData& DamageData);

	UFUNCTION(BlueprintCallable)
	float GetHealthCurrent() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	bool HasInfiniteHealth() const;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	void Kill(const FVector& DamageDirection, AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	void ResetToMax();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanReceiveDamage(const FMDamageInstanceData& Damage);

	UFUNCTION(BlueprintCallable)
	bool IsInvulnerable() const;

	UFUNCTION(BlueprintCallable)
	void AddInvulnerabilitySource(const UObject* Source);

	UFUNCTION(BlueprintCallable)
	void RemoveInvulnerabilitySource(const UObject* Source);

	UPROPERTY(BlueprintAssignable)
	FMOnDamagedSignature OnDamagedDelegate;

	UPROPERTY(BlueprintAssignable)
	FMOnHealedSignature OnHealedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FMOnDeathSignature OnDeathDelegate;

	UPROPERTY(BlueprintAssignable)
	FMOnHealthResetToMaxSignature OnHealthResetToMaxDelegate;

	UPROPERTY(BlueprintAssignable)
	FMInvulnerabilityStateChangedSignature OnInvulnerabilityStateChangedDelegate;

protected:
	UFUNCTION()
	void OnInvulnerabilityMultisourceLockableBoolValueChange(bool bIsActiveNew);

	// Config
	UPROPERTY(EditDefaultsOnly, Category = "Damageable")
	bool bInfiniteHealth;

	// Used to set initial Health and HealthMax attribute values
	UPROPERTY(EditDefaultsOnly, Category = "Damageable", meta = (EditCondition = "!bInfiniteHealth"))
	int32 HealthMaxInitial;

	UPROPERTY(EditDefaultsOnly, Category = "Damageable")
	TObjectPtr<UMDamageableOnDamageEffectSetupAsset> OnDamageEffectSetupAsset;
	// ~ Config

	// Default Components
	UPROPERTY(Transient)
	TObjectPtr<UMGameplayAttributeComponent> GameplayAttributeComponent;
	// ~ Default Components

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMDamageableOnDamageEffect_Base>> OnDamageEffects;

	UPROPERTY(Transient)
	TObjectPtr<UMMultisourceLockableBool> InvulnerabilityMultisourceLockableBool;
};
