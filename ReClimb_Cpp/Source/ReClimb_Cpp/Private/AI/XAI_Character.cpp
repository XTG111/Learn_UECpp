// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAI_Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Other/XLineBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/XSwordBase.h"
#include "Component/XPlayerStatsComponent.h"

// Sets default values
AXAI_Character::AXAI_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIStatesComponent = CreateDefaultSubobject<UXPlayerStatsComponent>(TEXT("AIStatesComponent"));
}

// Called when the game starts or when spawned
void AXAI_Character::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AXAI_Character::ReceivedDamage);
	//Patrol = Cast<AXLineBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AXLineBase::StaticClass()));
}

// Called every frame
void AXAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXAI_Character::ReceivedDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if (AIStatesComponent)
	{
		float curhealth = FMath::Clamp(AIStatesComponent->GetCurHealth() - Damage, -10.0f, AIStatesComponent->GetMaxHealth());
		if (curhealth <= 0.0f)
		{
			AIStatesComponent->SetCurHealth(0.0f);
			AIStatesComponent->SetIsDeath(true);
		}
		else
		{
			AIStatesComponent->SetCurHealth(curhealth);
			UE_LOG(LogTemp, Warning, TEXT("CurHealth:%f"), AIStatesComponent->GetCurHealth());
		}

	}
}

AXLineBase* AXAI_Character::GetPatrolRoute_Implementation()
{
	return Patrol;
}

float AXAI_Character::SetMovementSpeed_Implementation(EAIMovement SpeedEnum)
{
	float res = 0.0f;
	switch (SpeedEnum)
	{
	case EAIMovement::EAM_Idle:
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		res = 0.0f;
		break;
	case EAIMovement::EAM_Walking:
		GetCharacterMovement()->MaxWalkSpeed = 225.0f;
		res = 225.0f;
		break;
	case EAIMovement::EAM_Joggiing:
		GetCharacterMovement()->MaxWalkSpeed = 450.0f;
		res = 450.0f;
		break;
	case EAIMovement::EAM_Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = 675.0f;
		res = 675.0f;
		break;
	default:
		break;
	}
	return res;
}

void AXAI_Character::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 150.0f;
	DefendRadius = 350.0f;
}

void AXAI_Character::EquipWeapon_Implementation()
{

}

void AXAI_Character::UnEquipWeapon_Implementation()
{

}

void AXAI_Character::Attack_Implementation()
{
}

float AXAI_Character::GetCurrentHealth_Implementation()
{
	return AIStatesComponent->GetCurHealth();
}

float AXAI_Character::GetMaxHealth_Implementation()
{
	return AIStatesComponent->GetMaxHealth();
}

void AXAI_Character::Heal_Implementation(float healpercent)
{
	float healhp = FMath::Clamp(AIStatesComponent->GetCurHealth() + healpercent * AIStatesComponent->GetMaxHealth(), 0.0f, AIStatesComponent->GetMaxHealth());
	AIStatesComponent->SetCurHealth(healhp);
}

void AXAI_Character::SetIsWiledWeapon(bool bSet)
{
	bIsWiledWeapon = bSet;
}


