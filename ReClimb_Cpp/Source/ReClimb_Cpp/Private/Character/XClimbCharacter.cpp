// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XClimbCharacter.h"
#include "Character/XClimbCharacterAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Component/XClimbComponent.h"
#include "Component/XPlayerStatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/XAI_GunCharacter.h"


// Sets default values
AXClimbCharacter::AXClimbCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 225.0f;

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
	PlayerStatesComponent = CreateDefaultSubobject<UXPlayerStatsComponent>(TEXT("PlayerStatesComponent"));


	//TraceChannel AITarget
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AXClimbCharacter::DelayedStopMontageAndJumpPre);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AXClimbCharacter::JumpRe);

	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &AXClimbCharacter::FastSpeed);
	PlayerInputComponent->BindAction("Shift", IE_Released, this, &AXClimbCharacter::SlowSpeed);

	PlayerInputComponent->BindAction("Climb", IE_Pressed, this, &AXClimbCharacter::Climb);
	PlayerInputComponent->BindAction("Climb", IE_Released, this, &AXClimbCharacter::Climb);

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

void AXClimbCharacter::DelayedStopMontageAndJumpPre()
{
	//如果此时角色没有在攀爬
	if (!ClimbComponent->ClimbingCheck())
	{
		StopMontage(nullptr, 0.3f, 0.25f);
	}
	else
	{
		ClimbComponent->ClimbOrVault();
	}
}

void AXClimbCharacter::JumpRe()
{
	StopJumping();
}

void AXClimbCharacter::FastSpeed()
{
	bSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AXClimbCharacter::SlowSpeed()
{
	bSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AXClimbCharacter::Climb()
{
	bInClimb = true;
	ClimbComponent->ClimbOrVault();
}

void AXClimbCharacter::ReClimb()
{
	bInClimb = false;
}

void AXClimbCharacter::Attack()
{
	AXAI_GunCharacter* TargetActor = Cast<AXAI_GunCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AXAI_GunCharacter::StaticClass()));
	UGameplayStatics::ApplyDamage(TargetActor, 20.0f, nullptr, this, nullptr);
}

void AXClimbCharacter::DelayStopMontage(float MontageBlendOutTime, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	AnimInstance->Montage_Stop(MontageBlendOutTime, Montage);
	GetWorldTimerManager().ClearTimer(StopMontageTimerHandle);
	Jump();
}

void AXClimbCharacter::StopMontage(UAnimMontage* Montage, float DelayBeforeStoppingMontage, float MontageBlendOutTime)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	//如果传入的蒙太奇存在
	if (IsValid(Montage))
	{
		bool IsAnyMontagePlaying = AnimInstance->IsAnyMontagePlaying();
		//如果当前有蒙太奇动画的播放，那么延迟一段时间后停止播放
		if (IsAnyMontagePlaying)
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage Valid And Playing, need Delay!"));
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
			UE_LOG(LogTemp, Warning, TEXT("Montage Valid But not playing!"));
			Jump();
		}
	}
	//如果不存在蒙太奇，检测当前的蒙太奇播放序列
	else
	{
		bool IsAnyMontagePlaying = AnimInstance->IsAnyMontagePlaying();
		if (IsAnyMontagePlaying)
		{
			UE_LOG(LogTemp, Warning, TEXT("Now a montage Playing!"));
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
			UE_LOG(LogTemp, Warning, TEXT("No Montage!"));
			Jump();
		}
	}
}

void AXClimbCharacter::OnLanded(const FHitResult& Hit)
{
	ClimbComponent->ImpactLanding(Hit);
	//通过落地的速度操控角色的属性变化 生命和耐力
	AXClimbCharacter* CharacterRef = ClimbComponent->GetCharacter();
	float ZVelocity = CharacterRef->GetVelocity().Z;
	bool checkZvelocity = ZVelocity >= -1000.0f && ZVelocity <= -750.f;
	if (checkZvelocity)
	{
		float Health = PlayerStatesComponent->GetCurHealth() - 10.0f;
		float Stamina = PlayerStatesComponent->GetCurStamina() - 15.0f;
		PlayerStatesComponent->SetCurHealth(Health);
		PlayerStatesComponent->SetCurStamina(Stamina);
	}
	else
	{
		if (ZVelocity < -1000.0f)
		{
			float Health = PlayerStatesComponent->GetCurHealth() - 15.0f;
			float Stamina = PlayerStatesComponent->GetCurStamina() - 25.0f;
			PlayerStatesComponent->SetCurHealth(Health);
			PlayerStatesComponent->SetCurStamina(Stamina);
		}
	}
}


