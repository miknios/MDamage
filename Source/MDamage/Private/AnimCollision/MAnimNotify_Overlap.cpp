// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimCollision/MAnimNotify_Overlap.h"

#include "MDamageDataTypes.h"
#include "MDamageLibrary.h"
#include "MDebug.h"
#include "Engine/OverlapResult.h"

UMAnimNotify_OverlapHandler_Component::UMAnimNotify_OverlapHandler_Component()
{
}

void UMAnimNotify_OverlapHandler_Component::HandleAnimOverlap_Implementation(
	const TArray<FMAnimCollision_AnimNotifyHandlerOverlapData>& Overlaps, const FName OverlapId)
{
	FMAnimNotify_Overlap_OnOverlappedData OnAnimCollisionData;
	OnAnimCollisionData.OverlapId = OverlapId;
	OnAnimCollisionData.Overlaps = Overlaps;

	BP_OnAnimOverlap(OnAnimCollisionData);
	OnAnimOverlapDelegate.Broadcast(OnAnimCollisionData);
}

void UMAnimNotify_OverlapHandler_Component::BeginPlay()
{
	Super::BeginPlay();
}

void UAnimNotify_OverlapHandler_Base::HandleAnimOverlap_Implementation(
	AActor* Instigator, const TArray<FMAnimCollision_AnimNotifyHandlerOverlapData>& Overlaps)
{
}

#if WITH_EDITOR
bool UAnimNotify_OverlapHandler_Base::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* UAnimNotify_OverlapHandler_Base::GetWorld() const
{
	return Super::GetWorld();
}

void UAnimNotify_OverlapHandler_Damage::HandleAnimOverlap_Implementation(
	AActor* Instigator, const TArray<FMAnimCollision_AnimNotifyHandlerOverlapData>& Overlaps)
{
	Super::HandleAnimOverlap_Implementation(Instigator, Overlaps);

	if (Overlaps.IsEmpty())
	{
		return;
	}

	if (DamageConfigAsset == nullptr)
	{
		M::Debug::LogUserError(
			LogDamage, TEXT("Can't deal damage to actors overlapped from animation, because DamageConfigAsset is not set"),
			GetTypedOuter<UAnimSequence>());

		return;
	}

	for (const FMAnimCollision_AnimNotifyHandlerOverlapData& Overlap : Overlaps)
	{
		if (!Overlap.Actor.IsValid())
		{
			continue;
		}

		FVector DamageDirection = FVector::ZeroVector;
		if (Instigator != nullptr)
		{
			DamageDirection = Instigator->GetActorForwardVector();
		}

		FMDamageInstanceData DamageData = FMDamageInstanceData(DamageConfigAsset->DamageConfig, DamageDirection, Instigator);
		UMDamageLibrary::DealDamage(DamageData, Overlap.Actor.Get());
	}
}

void UAnimNotify_Overlap::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OverlappedActors.Empty();
}

void UAnimNotify_Overlap::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotify_Overlap::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float FrameDeltaTime,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	UWorld* World = MeshComp->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	const FTransform BoneTransform = MeshComp->GetBoneTransform(SocketName);

	const FCollisionShape CollisionShape = M::Physics::MakeCollisionShapeFromShapeRef(CollisionShapeRef);
	const FVector ColliderLocation = BoneTransform.GetLocation() + BoneTransform.TransformVector(LocationOffset);
	const FQuat ColliderQuat = BoneTransform.GetRotation();

	bool bOverlap = true;
	bool bDrawDebug = CVarShowDamageColliders.GetValueOnGameThread();

#if WITH_EDITOR
	bOverlap = World->IsGameWorld();
	bDrawDebug |= !World->IsGameWorld();
#endif


	if (bOverlap)
	{
		AActor* MeshOwner = MeshComp->GetOwner();
		if (!IsValid(MeshOwner))
		{
			return;
		}

		FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
		CollisionQueryParams.AddIgnoredActor(MeshOwner);

		// Ignore actors overlapped in previous frames
		CollisionQueryParams.AddIgnoredActors(OverlappedActors);

		TArray<FOverlapResult> OverlapResults;
		World->OverlapMultiByProfile(OverlapResults, ColliderLocation, ColliderQuat, CollisionProfile.Name,
		                             CollisionShape, CollisionQueryParams);

		if (OverlapResults.Num() > 0)
		{
			TArray<FMAnimCollision_AnimNotifyHandlerOverlapData> AnimCollisionOverlapData;
			for (auto OverlapResult : OverlapResults)
			{
				AActor* OverlappedActor = OverlapResult.GetActor();
				if (!IsValid(OverlappedActor))
				{
					continue;
				}

				if (OverlappedActors.Contains(OverlappedActor))
				{
					continue;
				}

				OverlappedActors.Emplace(OverlappedActor);
				AnimCollisionOverlapData.Emplace(FMAnimCollision_AnimNotifyHandlerOverlapData{OverlappedActor});
			}

			for (auto OverlapHandler : OverlapHandlers)
			{
				OverlapHandler->HandleAnimOverlap(MeshOwner, AnimCollisionOverlapData);
			}

			TArray<UActorComponent*> HandlerComponents =
				MeshOwner->GetComponentsByInterface(UMAnimNotify_Overlap_HandlerInterface::StaticClass());

			for (UActorComponent* HandlerComponent : HandlerComponents)
			{
				IMAnimNotify_Overlap_HandlerInterface::Execute_HandleAnimOverlap(
					HandlerComponent, AnimCollisionOverlapData, OverlapId);
			}
		}
	}

	if (bDrawDebug)
	{
		M::Physics::DrawCollisionShape(World, CollisionShape, FTransform(ColliderQuat, ColliderLocation), FColor::Red);
	}
}
