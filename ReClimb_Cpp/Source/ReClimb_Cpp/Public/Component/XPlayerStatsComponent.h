// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XPlayerStatsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCallOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCallOnDamageResponse, EDamageResponse, DamageResponse, AActor*, DamageCausor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCallOnBlocked, AActor*, DamageCauser, bool, bCanbeParried);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RECLIMB_CPP_API UXPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UXPlayerStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float MaxHealth = 100.0f;
	float MaxStamina = 100.0f;

	float CurHealth = 100.0f;
	float CurStamina = 100.0f;

	bool bIsDeath = false;
	bool bIsInvincible = false;
	bool bIsInterruptible = true;
	bool bIsBlocking = false;

	//多人攻击Token
	UPROPERTY(EditAnywhere)
		int AttackTokenCount = 1;


public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE void SetMaxHealth(float value) { MaxHealth = value; }
	FORCEINLINE void SetMaxStamina(float value) { MaxStamina = value; }

	FORCEINLINE float GetCurHealth() { return CurHealth; }
	FORCEINLINE float GetCurStamina() { return CurStamina; }
	void SetCurHealth(float value);
	FORCEINLINE void SetCurStamina(float value) { CurStamina = value; }

	FORCEINLINE bool GetIsDeath() { return bIsDeath; }
	FORCEINLINE void SetIsDeath(bool bdeath) { bIsDeath = bdeath; }

	FORCEINLINE bool GetIsBlocking() { return bIsBlocking; }
	FORCEINLINE void SetIsBlocking(bool bblockiing) { bIsBlocking = bblockiing; }

	FORCEINLINE bool GetbIsInterruptible() { return bIsInterruptible; }
	FORCEINLINE void SetbIsInterruptible(bool biinter) { bIsInterruptible = biinter; }

	FORCEINLINE int GetAttackTokenCount() { return AttackTokenCount; }
	FORCEINLINE void SetAttackTokenCount(int num) { AttackTokenCount = num; }

public:
	float Heal(float Amount);
	bool TakeDamage(struct FDamageInfo DamageInfo, AActor* DamageCausor);
	int CanBeDamaged(bool bShouldDamageInvincible, bool bCanbeBlocked);
	//检查当前的AttackToken是否多于正在请求的数量Amount
	bool ReserveAttackToken(int Amount);
	//当AI攻击完成后，返回Amount数量的Token
	void ReturnAttackToken(int Amount);

public:
	FCallOnDeath OnDeath;
	FCallOnDamageResponse OnDamageResponse;
	FCallOnBlocked OnBlocked;

};
