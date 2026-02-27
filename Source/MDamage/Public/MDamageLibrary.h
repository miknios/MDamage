// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MDamageLibrary.generated.h"


class UMDamageableComponent;
struct FMDamageInstanceData;

UCLASS()
class MDAMAGE_API UMDamageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MDamage")
	static UMDamageableComponent* GetDamageableComponent(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "MDamage")
	static void DealDamage(const FMDamageInstanceData& DamageInstanceData, AActor* Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MDamage")
	static bool IsDamageCollidersDebugEnabled();
};
