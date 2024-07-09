// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/XAI_Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "XAI_MageCharacter.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportEnd);

UCLASS()
class RECLIMB_CPP_API AXAI_MageCharacter : public AXAI_Character
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AXAI_MageCharacter();

public:
	// Called every frame
	float SetMovementSpeed_Implementation(EAIMovement SpeedEnum) override;
	void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;
	void Attack_Implementation(AActor* AttakTarget);

	UFUNCTION()
		void OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	//Teleport
	bool bInTeleport = false;
	void Teleport(FVector& TargetLoc);
	UFUNCTION()
		void MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	//Emitter
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* AttachParticle;
	UPROPERTY()
		class UParticleSystemComponent* TeleportBodyEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* MoveParticle;
	UPROPERTY()
		UParticleSystemComponent* TeleportTrailEffect;

	FOnTeleportEnd OnTeleportEnd;
	//Delay Destroy
	FTimerHandle ParticleDestroyTimer;
	UFUNCTION()
		void DestroyParticle();
};
