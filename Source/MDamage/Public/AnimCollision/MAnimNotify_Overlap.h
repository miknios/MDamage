// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPhysics.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MAnimNotify_Overlap.generated.h"

class UMDamageConfigAsset;

USTRUCT(BlueprintType)
struct FMAnimCollision_AnimNotifyHandlerOverlapData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> Actor;
};

UINTERFACE()
class UMAnimNotify_Overlap_HandlerInterface : public UInterface
{
	GENERATED_BODY()
};

class MDAMAGE_API IMAnimNotify_Overlap_HandlerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void HandleAnimOverlap(const TArray<FMAnimCollision_AnimNotifyHandlerOverlapData>& Overlaps, const FName OverlapId);
};

USTRUCT(BlueprintType)
struct FMAnimNotify_Overlap_Result
{
	GENERATED_BODY()

	FMAnimNotify_Overlap_Result() = default;

	FMAnimNotify_Overlap_Result(AActor* Actor)
		: Actor(Actor)
	{
	}

	UPROPERTY(BlueprintReadOnly)
	AActor* Actor = nullptr;
};

USTRUCT(BlueprintType)
struct FMAnimNotify_Overlap_OnOverlappedData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName OverlapId;

	UPROPERTY(BlueprintReadOnly)
	TArray<FMAnimCollision_AnimNotifyHandlerOverlapData> Overlaps;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSKOnAnimOverlapSignature, const FMAnimNotify_Overlap_OnOverlappedData&, OnAnimCollisionData);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MDAMAGE_API UMAnimNotify_OverlapHandler_Component : public UActorComponent, public IMAnimNotify_Overlap_HandlerInterface
{
	GENERATED_BODY()

public:
	UMAnimNotify_OverlapHandler_Component();

	// IMAnimCollision_AnimNotifyHandlerInterface
	virtual void HandleAnimOverlap_Implementation(const TArray<FMAnimCollision_AnimNotifyHandlerOverlapData>& Overlaps,
	                                              const FName OverlapId) override;
	// ~ IMAnimCollision_AnimNotifyHandlerInterface

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAnimOverlap(const FMAnimNotify_Overlap_OnOverlappedData& OnAnimCollisionData);

public:
	UPROPERTY(BlueprintAssignable)
	FSKOnAnimOverlapSignature OnAnimOverlapDelegate;

protected:
	virtual void BeginPlay() override;
};

UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class MDAMAGE_API UAnimNotify_OverlapHandler_Base : public UObject
{
	GENERATED_BODY()

	friend class UAnimNotify_Overlap;

public:
	// UObject
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
	virtual UWorld* GetWorld() const override;
	// ~ UObject

protected:
	UFUNCTION(BlueprintNativeEvent)
	void HandleAnimOverlap(AActor* Instigator, const TArray<FMAnimCollision_AnimNotifyHandlerOverlapData>& Overlaps);
};

UCLASS(DisplayName = "Deal Damage")
class UAnimNotify_OverlapHandler_Damage : public UAnimNotify_OverlapHandler_Base
{
	GENERATED_BODY()

public:
	virtual void HandleAnimOverlap_Implementation(AActor* Instigator,
	                                              const TArray<FMAnimCollision_AnimNotifyHandlerOverlapData>& Overlaps) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMDamageConfigAsset> DamageConfigAsset = nullptr;
};

/**
 * Find objects overlapped by defined collider over a set of frames
 * Tries to find the first mesh outer that implements IMAnimCollisionHandler to call HandleAnimOverlap on it
 * Collision is identified by OverlapId - handler should check for the id and decide what to do with the collision based on it
 */
UCLASS(meta = (DisplayName = "Overlap"))
class MDAMAGE_API UAnimNotify_Overlap : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OverlapId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMCollisionShapeRef CollisionShapeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AnimNotifyBoneName = "true"))
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCollisionProfileName CollisionProfile;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimNotify_OverlapHandler_Base>> OverlapHandlers;

	UPROPERTY()
	TArray<AActor*> OverlappedActors;
};
