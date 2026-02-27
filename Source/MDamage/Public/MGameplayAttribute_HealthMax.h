// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "MSimpleGAS/GameplayAttribute/MGameplayAttribute_FloatBase.h"
#include "MGameplayAttribute_HealthMax.generated.h"

UCLASS(NotBlueprintable, DisplayName = "Health Max")
class MDAMAGE_API UMGameplayAttribute_HealthMax final : public UMGameplayAttribute_FloatBase
{
	GENERATED_BODY()
	
	UMGameplayAttribute_HealthMax();
};
