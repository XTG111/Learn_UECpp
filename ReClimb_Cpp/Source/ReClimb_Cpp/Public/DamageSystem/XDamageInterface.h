// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Struct/XStructInfo.h"
#include "XDamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RECLIMB_CPP_API IXDamageInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetMaxHealth();
	//return IsDamaged
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool TakeDamage(FDamageInfo DamageInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float Heal(float Amount);
};
