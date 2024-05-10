// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/XCharacterInitInterface.h"
#include "XClimbComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RECLIMB_CPP_API UXClimbComponent : public UActorComponent, public IXCharacterInitInterface
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UXClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
		ACharacter* CharacterRef;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere)
		class UCharacterMovementComponent* CharacterMovementIns;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* FPMeshIns;

	//攀爬状态设置
	bool bClimbing;

	//重写IXCharacterInitInterface接口类中的函数
public:
	virtual void InitCharacter(ACharacter* Player, UArrowComponent* WallTraceArrow, UCapsuleComponent* Capsule, UCharacterMovementComponent* CharacterMovement) override;
	virtual void StopMontage() override;
	virtual void SetFPMesh(USkeletalMeshComponent* FPMesh) override;

public:
	//判断是否正在攀爬
	bool ClimbingCheck();

};
