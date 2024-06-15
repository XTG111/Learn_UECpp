// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XPlayerStatsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCallOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCallOnDamageResponse, EDamageResponse, DamageResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCallOnBlocked, bool, bCanbeParried);

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

public:
	inline float GetMaxHealth() { return MaxHealth; }
	inline float GetMaxStamina() { return MaxStamina; }
	inline void SetMaxHealth(float value) { MaxHealth = value; }
	inline void SetMaxStamina(float value) { MaxStamina = value; }

	inline float GetCurHealth() { return CurHealth; }
	inline float GetCurStamina() { return CurStamina; }
	void SetCurHealth(float value);
	inline void SetCurStamina(float value) { CurStamina = value; }

	inline bool GetIsDeath() { return bIsDeath; }
	inline void SetIsDeath(bool bdeath) { bIsDeath = bdeath; }

	inline bool GetIsBlocking() { return bIsBlocking; }
	inline void SetIsBlocking(bool bblockiing) { bIsBlocking = bblockiing; }

public:
	float Heal(float Amount);
	bool TakeDamage(struct FDamageInfo DamageInfo);
	int CanBeDamaged(bool bShouldDamageInvincible, bool bCanbeBlocked);

public:
	FCallOnDeath OnDeath;
	FCallOnDamageResponse OnDamageResponse;
	FCallOnBlocked OnBlocked;

};
