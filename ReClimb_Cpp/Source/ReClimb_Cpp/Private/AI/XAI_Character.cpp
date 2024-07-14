// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAI_Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Other/XLineBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/XSwordBase.h"
#include "Component/XPlayerStatsComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/XWidget_EnemyHeadHP.h"
#include "Components/CapsuleComponent.h"
#include "Struct/XStructInfo.h"
#include "Components/ProgressBar.h"
#include "AI/XAIController.h"
#include "Component/XCombatComponent.h"


// Sets default values
AXAI_Character::AXAI_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIStatesComponent = CreateDefaultSubobject<UXPlayerStatsComponent>(TEXT("AIStatesComponent"));
	CombatComponent = CreateDefaultSubobject<UXCombatComponent>(TEXT("CombatComponent"));

	EnemyHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	EnemyHPWidget->SetupAttachment(GetMesh());
	EnemyHPWidget->SetDrawSize(FVector2D(200, 20)); // Example size
	EnemyHPWidget->SetWidgetSpace(EWidgetSpace::Screen);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

// Called when the game starts or when spawned
void AXAI_Character::BeginPlay()
{
	Super::BeginPlay();
	//Patrol = Cast<AXLineBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AXLineBase::StaticClass()));
	if (HPWidget)
	{
		EnemyHPWidget->SetWidget(CreateWidget<UUserWidget>(GetWorld(), HPWidget));
	}
	if (AIStatesComponent)
	{
		AIStatesComponent->OnDeath.AddDynamic(this, &AXAI_Character::CallOnDeath);
		AIStatesComponent->OnBlocked.AddDynamic(this, &AXAI_Character::CallOnBlocked);
		AIStatesComponent->OnDamageResponse.AddDynamic(this, &AXAI_Character::CallOnDamageResponse);
	}
	AIController = Cast<AXAIController>(GetController());
}

// Called every frame
void AXAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void AXAI_Character::Attack_Implementation(AActor* AttakTarget)
{
	bAttacking = true;
	AttackTargetActor = AttakTarget;
}

void AXAI_Character::JumpToLoc_Implementation(FVector Location)
{
	//UE_LOG(LogTemp, Warning, TEXT("JumpToLoc"));
	FVector LaunchVelocity;
	FVector EndLoc = FVector{ Location.X,Location.Y,Location.Z + 250.f };
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		GetWorld(),
		LaunchVelocity,
		GetActorLocation(),
		EndLoc,
		0.0f,
		0.5f
	);
	//以一定速度将Actor发射出去
	LaunchCharacter(LaunchVelocity, true, true);
}

float AXAI_Character::GetCurrentHealth_Implementation()
{
	return AIStatesComponent->GetCurHealth();
}

float AXAI_Character::GetMaxHealth_Implementation()
{
	return AIStatesComponent->GetMaxHealth();
}

bool AXAI_Character::TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCausor)
{
	return AIStatesComponent->TakeDamage(DamageInfo, DamageCausor);
}

float AXAI_Character::Heal_Implementation(float Amount)
{
	float value = AIStatesComponent->Heal(Amount);
	float MaxHealth = AIStatesComponent->GetMaxHealth();
	UXWidget_EnemyHeadHP* Widget = Cast<UXWidget_EnemyHeadHP>(EnemyHPWidget->GetWidget());
	if (Widget)
	{
		Widget->HealthBar->SetPercent(value / MaxHealth);
	}
	return value;
}

bool AXAI_Character::IsDead_Implementation()
{
	return AIStatesComponent->GetIsDeath();
}

bool AXAI_Character::IsAttacking_Implementation()
{
	return bAttacking;
}

bool AXAI_Character::ReserveAttackToken_Implementation(int Amount)
{
	return AIStatesComponent->ReserveAttackToken(Amount);
}

void AXAI_Character::ReturnAttackToken_Implementation(int Amount)
{
	AIStatesComponent->ReturnAttackToken(Amount);
}

void AXAI_Character::SetIsInteruptible_Implementation(bool binter)
{
	AIStatesComponent->SetbIsInterruptible(binter);
}

int AXAI_Character::GetTeamNumber_Implementation()
{
	return TeamNum;
}

bool AXAI_Character::AttackStart_Implementation(AActor* AttackTarget, int TokenNeeded)
{
	//校验是否有足够的Token可以供该AI使用攻击
	if (AttackTarget && AttackTarget->Implements<UXDamageInterface>())
	{
		if (IXDamageInterface::Execute_ReserveAttackToken(AttackTarget, TokenNeeded))
		{
			//当有足够的Token将进行攻击，并且存储Token值
			IXAIInterface::Execute_StoreAttackToken(this, AttackTarget, TokenNeeded);
			CurAttackNeedToken = TokenNeeded;
			return true;
		}
		else return false;
	}
	return false;
}

void AXAI_Character::AttackEnd_Implementation(AActor* AttackTarget)
{
	if (AttackTarget && AttackTarget->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_ReturnAttackToken(AttackTarget, CurAttackNeedToken);
		int NeedRemoveTokenFromMap = -1 * CurAttackNeedToken;
		IXAIInterface::Execute_StoreAttackToken(this, AttackTarget, NeedRemoveTokenFromMap);
	}
	bAttacking = false;
	CallOnAttackEndCall.Broadcast();
}

void AXAI_Character::StoreAttackToken_Implementation(AActor* AttackTarget, int TokenNeeded)
{
	if (CostTokenForTarget.Find(AttackTarget))
	{
		CostTokenForTarget[AttackTarget] += TokenNeeded;
	}
	else
	{
		CostTokenForTarget.Add(AttackTarget, TokenNeeded);
	}
}

void AXAI_Character::CallOnDeath()
{
	//UE_LOG(LogTemp, Warning, TEXT("AI On Death"));
	StopAnimMontage();
	UXWidget_EnemyHeadHP* Widget = Cast<UXWidget_EnemyHeadHP>(EnemyHPWidget->GetWidget());
	if (Widget)
	{
		Widget->HealthBar->SetPercent(0);
	}
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AIStatesComponent->SetIsDeath(true);
	FString Reason = TEXT("Dead");
	AIController->GetBrainComponent()->StopLogic(Reason);
	AIController->SetStateAsDead();
	Widget->SetVisibility(ESlateVisibility::Hidden);
	for (auto& pair : CostTokenForTarget)
	{
		AActor* actor = pair.Key;
		if (actor && actor->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_ReturnAttackToken(actor, pair.Value);
		}
	}
}


void AXAI_Character::CallOnBlocked(AActor* DamageCauser, bool bCanbeParried)
{
	//IXDamageInterface::Execute_ReturnAttackToken(AttackTargetActor, CurAttackNeedToken);
}

void AXAI_Character::CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor)
{
	MakeDamageActor = DamageCausor;
	float value = AIStatesComponent->GetCurHealth();
	float MaxHealth = AIStatesComponent->GetMaxHealth();
	UXWidget_EnemyHeadHP* Widget = Cast<UXWidget_EnemyHeadHP>(EnemyHPWidget->GetWidget());
	if (Widget)
	{
		Widget->HealthBar->SetPercent(value / MaxHealth);
	}

	GetCharacterMovement()->StopMovementImmediately();
	AIController->SetStateAsFrozen();
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	PlayAnimMontage(HitMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_Character::OnHitMontageEnd);

	UE_LOG(LogTemp, Warning, TEXT("AI On DamageResponse"));
}

void AXAI_Character::OnHitMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	int DamageActorNum = 0;
	if (MakeDamageActor && MakeDamageActor->Implements<UXDamageInterface>())
	{
		DamageActorNum = IXDamageInterface::Execute_GetTeamNumber(MakeDamageActor);
	}
	if (DamageActorNum != TeamNum) AIController->SetStateAsAttacking(MakeDamageActor, true);
	else AIController->SetStateAsAttacking(nullptr, true);
}

void AXAI_Character::SetIsWiledWeapon(bool bSet)
{
	bIsWiledWeapon = bSet;
}


