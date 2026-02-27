// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MDamageDebugSubsystem.generated.h"

/**
 * Draws debug health bar over all actors with MDamageableComponent attached
 */
UCLASS()
class MDAMAGE_API UMDamageDebugSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	// UTickableWorldSubsystem
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	// ~ UTickableWorldSubsystem
};
