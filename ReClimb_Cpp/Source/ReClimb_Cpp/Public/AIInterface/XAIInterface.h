// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "D:/UnrealProject/Learn_UECpp/Learn_UECpp/ReClimb_Cpp/Source/ReClimb_Cpp/XHeadFile/XEnumType.h"
#include "XAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RECLIMB_CPP_API IXAIInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		class AXLineBase* GetPatrolRoute();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float SetMovementSpeed(EAIMovement SpeedEnum);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void GetIdealRange(float& AttackRadius, float& DefendRadius);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void EquipWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void UnEquipWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Attack();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetMaxHealth();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Heal(float healpercent);
};
