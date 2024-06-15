// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widget/XHUD.h"
#include "Struct/XStructInfo.h"
#include "XCombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RECLIMB_CPP_API UXCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UXCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
		class AXClimbCharacter* CharacterEx;
	//HUD
	UPROPERTY()
		class AXHUD* XCharacterHUD;
	UPROPERTY()
		class APlayerController* PIC;

	//准星
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairCenter;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairLeft;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairRight;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairTop;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairBottom;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		FHUDPackage HUDPackage;

	void SetHUDCrossHair(float delta);

	//spawn MagicSpell
public:

	//接受伤害事件
	FOnAttackEnded OnAttackEnded;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AXProjectilesBase> ProjectileEx;
	void MagicSpell(FTransform& spawntransform, AActor* TargetActor, FDamageInfo damageinfo);
	void FireBullet(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor);
	void SwordAttack(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor);
	//攻击信息
	FDamageInfo DamageInfo;
	//绑定击中函数
	UFUNCTION()
		void OnProjectileHit(AActor* OtherActor, const FHitResult& HitRes);
};
