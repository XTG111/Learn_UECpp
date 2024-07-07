// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/XPlayerStatsComponent.h"
#include "AI/XAI_Character.h"
#include "Struct/XStructInfo.h"

// Sets default values for this component's properties
UXPlayerStatsComponent::UXPlayerStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}


// Called every frame
void UXPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UXPlayerStatsComponent::SetCurHealth(float value)
{
	CurHealth = value;
}

float UXPlayerStatsComponent::Heal(float Amount)
{
	if (bIsDeath) return 0.0f;
	CurHealth = FMath::Clamp(CurHealth + Amount, 0.0f, MaxHealth);
	return CurHealth;
}

bool UXPlayerStatsComponent::TakeDamage(FDamageInfo DamageInfo, AActor* DamageCausor)
{
	int check = CanBeDamaged(DamageInfo.bShouldDamageInvincible, DamageInfo.bCanBeBlocked);
	if (check == 0)
	{
		OnBlocked.Broadcast(DamageInfo.bCanBeParried);
		return false;
	}
	else if (check == 1)
	{
		CurHealth = FMath::Clamp(CurHealth - DamageInfo.Amount, 0.0f, MaxHealth);
		if (CurHealth <= 0.0f)
		{
			bIsDeath = true;
			OnDeath.Broadcast();
			return true;
		}
		else
		{
			//检测该伤害是否可以被打断
			if (bIsInterruptible || DamageInfo.bShouldForceInterrupt)
			{
				OnDamageResponse.Broadcast(DamageInfo.DamageResponse, DamageCausor);
				return true;
			}
			else return false;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("NO DAMAGED"));
		return false;
	}
}

int UXPlayerStatsComponent::CanBeDamaged(bool bShouldDamageInvincible, bool bCanbeBlocked)
{
	bool check = !bIsDeath || !bIsInvincible || bShouldDamageInvincible;
	if (check)
	{
		if (bIsBlocking && bCanbeBlocked) return 0; //BlockDamage
		else return 1; // Do Damage
	}
	else return 2; //No Damage
}

bool UXPlayerStatsComponent::ReserveAttackToken(int Amount)
{
	if (AttackTokenCount >= Amount)
	{
		AttackTokenCount -= Amount;
		return true;
	}
	else return false;
}

void UXPlayerStatsComponent::ReturnAttackToken(int Amount)
{
	AttackTokenCount += Amount;
}

