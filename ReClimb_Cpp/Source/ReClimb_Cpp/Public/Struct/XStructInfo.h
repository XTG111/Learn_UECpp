// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "ReClimb_Cpp/XHeadFile/XEnumType.h"
#include "XStructInfo.generated.h"


//存储攀爬动画的结构体
USTRUCT(BlueprintType)
struct FClimbAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClimbAnim")
		TArray<class UAnimMontage*> HighClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClimbAnim")
		TArray<UAnimMontage*> RunHighClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClimbAnim")
		TArray<UAnimMontage*> LowClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClimbAnim")
		TArray<UAnimMontage*> RunLowClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClimbAnim")
		TArray<UAnimMontage*> JumpClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClimbAnim")
		TArray<UAnimMontage*> RunJumpClimb;
};

//存储落地动画的结构体
USTRUCT(BlueprintType)
struct FLandAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> LowLand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> HighLand;
	//翻越后落地动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> LowVaultLand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> HighVaultLand;
};

//存储翻越动画的结构体
USTRUCT(BlueprintType)
struct FVaultAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VaultAnim")
		TArray<UAnimMontage*> LowVault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VaultAnim")
		TArray<UAnimMontage*> RunLowVault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VaultAnim")
		TArray<UAnimMontage*> HighVault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VaultAnim")
		TArray<UAnimMontage*> HighVault_JumpDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VaultAnim")
		TArray<UAnimMontage*> HighFenceVault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VaultAnim")
		TArray<UAnimMontage*> JumpFenceVault;
};

//三种结构体的结构体
USTRUCT(BlueprintType)
struct FAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FClimbAnims ClimbAnims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FLandAnims LandAnims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FVaultAnims VaultAnims;
};

//DamageInfo
USTRUCT(BlueprintType)
struct FDamageInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		EDamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		EDamageResponse DamageResponse;
	//无法躲避该伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool bShouldDamageInvincible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool bCanBeBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool bCanBeParried;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool bShouldForceInterrupt;
};

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXStructInfo : public UUserDefinedStruct
{
	GENERATED_BODY()

};
