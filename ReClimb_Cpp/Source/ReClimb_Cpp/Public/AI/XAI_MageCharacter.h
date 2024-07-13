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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealOverTimeEnd);
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
	//void Attack_Implementation(AActor* AttakTarget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HealAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* NormalAttackAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* SmashMontage;

	UPROPERTY(EditAnywhere, Category = "AttackProj")
		TSubclassOf<class AXProjectilesBase> AttackProjectileEx;
	UPROPERTY(EditAnywhere, Category = "AttackProj")
		TSubclassOf<class AXProjectilesBase> NormalProjectileEx;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AXAOEBase> HealAOEClass;

	UPROPERTY()
		class AXAOE_Heal* HealAOEActor;

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
	FOnHealOverTimeEnd OnHealOverTimeEnd;
	//Delay Destroy
	FTimerHandle ParticleDestroyTimer;
	UFUNCTION()
		void DestroyParticle();

	//Heal
	void HealOverTime();
	UFUNCTION()
		void HealEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void HealActor(AActor* actor);

	//Attack Type
	void BaseAttack(AActor* AttakTarget);
	void BarrageAttack(AActor* AttakTarget);

	//Smash Attack
	void SmashAttack(AActor* AttakTarget);
};
