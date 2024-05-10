// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XCharacterInitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXCharacterInitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RECLIMB_CPP_API IXCharacterInitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		virtual void InitCharacter(ACharacter* Player,class UArrowComponent* WallTraceArrow, class UCapsuleComponent* Capsule, class UCharacterMovementComponent* CharacterMovement) = 0;
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		virtual void StopMontage() = 0;
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		virtual void SetFPMesh(class USkeletalMeshComponent* FPMesh) = 0;
};
