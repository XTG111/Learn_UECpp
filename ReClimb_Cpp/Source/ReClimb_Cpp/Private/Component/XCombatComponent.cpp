// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/XCombatComponent.h"
#include "Character/XClimbCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/XProjectiles_Dark.h"
#include "DamageSystem/XDamageInterface.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UXCombatComponent::UXCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterEx = Cast<AXClimbCharacter>(GetOwner());
	// ...
}


// Called every frame
void UXCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetHUDCrossHair(DeltaTime);
	// ...
}

void UXCombatComponent::SetHUDCrossHair(float delta)
{
	if (CharacterEx == nullptr || CharacterEx->Controller == nullptr) return;
	PIC = PIC == nullptr ? Cast<APlayerController>(CharacterEx->Controller) : PIC;
	if (PIC)
	{
		XCharacterHUD = XCharacterHUD == nullptr ? Cast<AXHUD>(PIC->GetHUD()) : XCharacterHUD;
		if (XCharacterHUD)
		{
			if (CharacterEx->GetPlayerStance() == EPlayerStance::EPS_Default)
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}
			else if (CharacterEx->GetPlayerStance() == EPlayerStance::EPS_Magic)
			{
				HUDPackage.CrosshairsCenter = CrosshairCenter;
				HUDPackage.CrosshairsLeft = CrosshairLeft;
				HUDPackage.CrosshairsRight = CrosshairRight;
				HUDPackage.CrosshairsTop = CrosshairTop;
				HUDPackage.CrosshairsBottom = CrosshairBottom;
			}
			else return;
			HUDPackage.CrosshairColor = FColor::White;
			XCharacterHUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UXCombatComponent::MagicSpell(FTransform& spawntransform, AActor* TargetActor, FDamageInfo damageinfo)
{
	DamageInfo = damageinfo;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	AXProjectiles_Dark* SpawnActor = GetWorld()->SpawnActor<AXProjectiles_Dark>(ProjectileEx, spawntransform, SpawnParameters);
	SpawnActor->TargetActor = TargetActor;
	SpawnActor->Speed = 1000.0f;
	SpawnActor->SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	SpawnActor->OnProjectileImpact.AddDynamic(this, &UXCombatComponent::OnProjectileHit);
}

void UXCombatComponent::OnProjectileHit(AActor* OtherActor, const FHitResult& HitRes)
{
	if (OtherActor && OtherActor->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_TakeDamage(OtherActor, DamageInfo, GetOwner());
	}

	//对应蓝图中的ReportDamageEvent
	FAIDamageEvent DamageEvent;

	DamageEvent.Instigator = GetOwner();
	DamageEvent.Amount = DamageInfo.Amount;
	DamageEvent.Location = GetOwner()->GetActorLocation();
	DamageEvent.DamagedActor = OtherActor;

	UAIPerceptionSystem::GetCurrent(GetWorld())->OnEvent(DamageEvent);

	OnAttackEnded.Broadcast();
}

void UXCombatComponent::FireBullet(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor)
{
	ETraceTypeQuery ETType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<FHitResult> HitResults;
	bool bIsHit = UKismetSystemLibrary::LineTraceMulti(
		GetWorld(),
		TraceStart,
		TraceEnd,
		ETType,
		false,
		TArray<AActor*>{IgnoreActor},
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	if (bIsHit)
	{
		AActor* DamageActor = DamageFirstNonTeamActor(HitResults);
		if (DamageActor && DamageActor->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_TakeDamage(DamageActor, damageinfo, GetOwner());
		}

		FAIDamageEvent DamageEvent;

		DamageEvent.Instigator = GetOwner();
		DamageEvent.Amount = DamageInfo.Amount;
		DamageEvent.Location = GetOwner()->GetActorLocation();
		DamageEvent.DamagedActor = DamageActor;

		UAIPerceptionSystem::GetCurrent(GetWorld())->OnEvent(DamageEvent);
	}
}

void UXCombatComponent::SwordAttack(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor)
{
	ETraceTypeQuery ETType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<FHitResult> HitResults;
	bool bIsHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		TraceStart,
		TraceEnd,
		20.0f,
		ETType,
		false,
		TArray<AActor*>{IgnoreActor},
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bIsHit)
	{
		TArray<AActor*> DamageActors = DamageAllNonTeamActor(HitResults);
		for (auto& DamageActor : DamageActors)
		{
			if (DamageActor && DamageActor->Implements<UXDamageInterface>())
			{
				IXDamageInterface::Execute_TakeDamage(DamageActor, damageinfo, GetOwner());
			}

			FAIDamageEvent DamageEvent;

			DamageEvent.Instigator = GetOwner();
			DamageEvent.Amount = DamageInfo.Amount;
			DamageEvent.Location = GetOwner()->GetActorLocation();
			DamageEvent.DamagedActor = DamageActor;

			UAIPerceptionSystem::GetCurrent(GetWorld())->OnEvent(DamageEvent);
		}
	}
}

TArray<AActor*> UXCombatComponent::DamageAllNonTeamActor(TArray<FHitResult>& HitResults)
{
	TArray<AActor*> Res;
	int DamageActorNum = 0;
	int SelfActorNum = 0;
	for (auto& HitRes : HitResults)
	{
		AActor* DamageActor = Cast<AActor>(HitRes.Actor);
		if (DamageActor && DamageActor->Implements<UXDamageInterface>())
		{
			DamageActorNum = IXDamageInterface::Execute_GetTeamNumber(DamageActor);
		}
		if (GetOwner() && GetOwner()->Implements<UXDamageInterface>())
		{
			SelfActorNum = IXDamageInterface::Execute_GetTeamNumber(GetOwner());
		}
		if (DamageActorNum != SelfActorNum)
		{
			Res.AddUnique(DamageActor);
		}
	}
	return Res;
}

AActor* UXCombatComponent::DamageFirstNonTeamActor(TArray<FHitResult>& HitResults)
{
	int DamageActorNum = 0;
	int SelfActorNum = 0;
	for (auto& HitRes : HitResults)
	{
		AActor* DamageActor = Cast<AActor>(HitRes.Actor);
		if (DamageActor && DamageActor->Implements<UXDamageInterface>())
		{
			DamageActorNum = IXDamageInterface::Execute_GetTeamNumber(DamageActor);
		}
		if (GetOwner() && GetOwner()->Implements<UXDamageInterface>())
		{
			SelfActorNum = IXDamageInterface::Execute_GetTeamNumber(GetOwner());
		}
		if (DamageActorNum != SelfActorNum)
		{
			return DamageActor;
			break;
		}
	}
	return nullptr;
}


