// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XClimbCharacter.h"
#include "Character/XClimbCharacterAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Component/XClimbComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"


// Sets default values
AXClimbCharacter::AXClimbCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TP_CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TP_CameraSpringArm"));
	TP_CameraSpringArm->SetupAttachment(GetCapsuleComponent());
	TP_CameraSpringArm->TargetArmLength = 600.0f;
	TP_CameraSpringArm->bUsePawnControlRotation = true;
	TP_CameraSpringArm->SocketOffset = FVector{ 0.0f,0.0f,250.0f };


	TP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TP_Camera"));	
	TP_Camera->SetupAttachment(TP_CameraSpringArm, USpringArmComponent::SocketName);
	TP_Camera->bUsePawnControlRotation = false;

	WallTraceArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("WallTraceArrow"));
	WallTraceArrow->SetupAttachment(GetMesh());
	WallTraceArrow->SetArrowColor(FColor::Red);

	ClimbComponent = CreateDefaultSubobject<UXClimbComponent>(TEXT("ClimbComponent"));
	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called when the game starts or when spawned
void AXClimbCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ClimbComponent)
	{
		ClimbComponent->InitCharacter(this, WallTraceArrow, GetCapsuleComponent(), GetCharacterMovement());
		ClimbComponent->SetFPMesh(nullptr);
	}
}

// Called every frame
void AXClimbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXClimbCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AXClimbCharacter::MoveForWard);
	PlayerInputComponent->BindAxis("MoveRight", this, &AXClimbCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AXClimbCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AXClimbCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AXClimbCharacter::JumpPre);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AXClimbCharacter::JumpRe);

}

void AXClimbCharacter::MoveForWard(float value)
{
	AxisValue = value;
	FRotator Rotation = GetControlRotation();
	Rotation.Roll = 0.0f;
	Rotation.Pitch = 0.0f;
	FVector FVector = UKismetMathLibrary::GetForwardVector(Rotation);
	AddMovementInput(FVector, value, false);
}

void AXClimbCharacter::MoveRight(float value)
{
	AxisValue = value;
	FRotator Rotation = GetControlRotation();
	Rotation.Roll = 0.0f;
	Rotation.Pitch = 0.0f;
	FVector FVector = UKismetMathLibrary::GetRightVector(Rotation);
	AddMovementInput(FVector, value, false);
}

void AXClimbCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AXClimbCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void AXClimbCharacter::JumpPre()
{
	Jump();
	//如果此时角色没有在攀爬
	if (ClimbComponent && !ClimbComponent->ClimbingCheck())
	{
		StopMontage(nullptr, 0.3f, 0.25f);
		if(bCanChangeMontage) Jump();
	}
}

void AXClimbCharacter::JumpRe()
{
	StopJumping();
}

void AXClimbCharacter::DelayStopMontage(float MontageBlendOutTime, UAnimMontage* Montage)
{
	if (!AnimInstance) return;
	AnimInstance->Montage_Stop(MontageBlendOutTime, Montage);
	GetWorldTimerManager().ClearTimer(StopMontageTimerHandle);
	bCanChangeMontage = true;
}

void AXClimbCharacter::StopMontage(UAnimMontage* Montage, float DelayBeforeStoppingMontage, float MontageBlendOutTime)
{
	if (!AnimInstance) return;
	//如果传入的蒙太奇存在
	if (IsValid(Montage))
	{
		bool IsAnyMontagePlaying = AnimInstance->IsAnyMontagePlaying();
		//如果当前有蒙太奇动画的播放，那么延迟一段时间后停止播放
		if (IsAnyMontagePlaying)
		{
			StopMontageTimerDelegate.BindUFunction(this, FName("DelayStopMontage"), MontageBlendOutTime, Montage);
			
			GetWorldTimerManager().SetTimer(
				StopMontageTimerHandle,
				StopMontageTimerDelegate,
				DelayBeforeStoppingMontage,
				false
				);
		}
		else
		{
			bCanChangeMontage = true;
		}
	}
	//如果不存在蒙太奇，检测当前的蒙太奇播放序列
	else
	{
		bool IsAnyMontagePlaying = AnimInstance->IsAnyMontagePlaying();
		if (IsAnyMontagePlaying)
		{
			float blendtime = 0.3f;
			UAnimMontage* montage = AnimInstance->GetCurrentActiveMontage();
			StopMontageTimerDelegate.BindUFunction(this, FName("DelayStopMontage"), blendtime, montage);

			GetWorldTimerManager().SetTimer(
				StopMontageTimerHandle,
				StopMontageTimerDelegate,
				DelayBeforeStoppingMontage,
				false
			);
		}
		else
		{
			bCanChangeMontage = true;
		}
	}
}


