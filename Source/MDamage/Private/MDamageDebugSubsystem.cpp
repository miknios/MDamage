// Copyright (c) Miknios. All rights reserved.


#include "MDamageDebugSubsystem.h"

#include "EngineUtils.h"
#include "MDamageableComponent.h"
#include "MDamageLibrary.h"

TAutoConsoleVariable<bool> CVarShowDamageDebugs(TEXT("m.Damage.ShowDebugs"), false, TEXT("Show Damageable debug info"));

bool UMDamageDebugSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

TStatId UMDamageDebugSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(MDamageDebugSubsystem, STATGROUP_Tickables);
}

bool UMDamageDebugSubsystem::IsTickable() const
{
	return CVarShowDamageDebugs.GetValueOnAnyThread();
}

void UMDamageDebugSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const UWorld* World = GetWorld();

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor))
		{
			continue;
		}

		UMDamageableComponent* DamageableComponent = UMDamageLibrary::GetDamageableComponent(Actor);
		if (!IsValid(DamageableComponent))
		{
			continue;
		}

		const bool bInfiniteHealth = DamageableComponent->HasInfiniteHealth();
		const int HealthMax = DamageableComponent->GetHealthMax();
		const int HealthCurrent = DamageableComponent->GetHealthCurrent();

		FString DebugString = bInfiniteHealth ? TEXT("Infinite Health") : FString::Printf(TEXT("%d / %d"), HealthCurrent, HealthMax);
		if (DamageableComponent->IsInvulnerable())
		{
			DebugString += TEXT(" (Invulnerable)");
		}

		DrawDebugString(World, Actor->GetActorLocation(), DebugString, 0, FColor::Red, 0.0f,
		                1.0f, 2);
	}
}
