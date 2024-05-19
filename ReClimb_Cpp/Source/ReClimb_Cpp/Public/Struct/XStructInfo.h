// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "XStructInfo.generated.h"


//�洢���������Ľṹ��
USTRUCT(BlueprintType)
struct FClimbAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

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

//�洢��ض����Ľṹ��
USTRUCT(BlueprintType)
struct FLandAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> LowLand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> HighLand;
	//��Խ����ض���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> LowVaultLand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandAnim")
		TArray<UAnimMontage*> HighVaultLand;
};

//�洢��Խ�����Ľṹ��
USTRUCT(BlueprintType)
struct FVaultAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

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

//���ֽṹ��Ľṹ��
USTRUCT(BlueprintType)
struct FAnims : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FClimbAnims ClimbAnims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FLandAnims LandAnims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FVaultAnims VaultAnims;
};

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXStructInfo : public UUserDefinedStruct
{
	GENERATED_BODY()

};
