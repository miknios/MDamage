// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "MSimpleGAS/GameplayAttribute/MGameplayAttribute_FloatBase.h"
#include "MGameplayAttribute_Health.generated.h"

UCLASS(NotBlueprintable, DisplayName = "Health")
class MDAMAGE_API UMGameplayAttribute_Health final : public UMGameplayAttribute_FloatBase
{
	GENERATED_BODY()
	
	UMGameplayAttribute_Health();
};
