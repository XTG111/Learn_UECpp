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
#include "DamageSystem/XDamageComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/XWidget_EnemyHeadHP.h"
#include "Components/ProgressBar.h"
#include "Component/XCombatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Sets default values
AXClimbCharacter::AXClimbCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

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
	CombatComponent = CreateDefaultSubobject<UXCombatComponent>(TEXT("CombatComponent"));

	//TraceChannel AITarget
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);


	CharacterHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	CharacterHPWidget->SetupAttachment(GetMesh());
	CharacterHPWidget->SetDrawSize(FVector2D(200, 20)); // Example size
	CharacterHPWidget->SetWidgetSpace(EWidgetSpace::Screen);

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
	if (PlayerStatesComponent)
	{
		PlayerStatesComponent->OnDeath.AddDynamic(this, &AXClimbCharacter::CallOnDeath);
		PlayerStatesComponent->OnBlocked.AddDynamic(this, &AXClimbCharacter::CallOnBlocked);
		PlayerStatesComponent->OnDamageResponse.AddDynamic(this, &AXClimbCharacter::CallOnDamageResponse);
	}
	if (HPWidget)
	{
		CharacterHPWidget->SetWidget(CreateWidget<UUserWidget>(GetWorld(), HPWidget));
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
	//PlayerInputComponent->BindAction("Climb", IE_Released, this, &AXClimbCharacter::Climb);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AXClimbCharacter::Attack);
	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &AXClimbCharacter::DoHeal);

	PlayerInputComponent->BindAction("ChangeStance", IE_Pressed, this, &AXClimbCharacter::MagicStance);
	PlayerInputComponent->BindAction("ChangeStance", IE_Released, this, &AXClimbCharacter::DefaultStance);

}

float AXClimbCharacter::GetCurrentHealth_Implementation()
{
	return PlayerStatesComponent->GetCurHealth();
}

float AXClimbCharacter::GetMaxHealth_Implementation()
{
	return PlayerStatesComponent->GetMaxHealth();
}

bool AXClimbCharacter::TakeDamage_Implementation(FDamageInfo DamageInfo)
{
	return PlayerStatesComponent->TakeDamage(DamageInfo);
}

float AXClimbCharacter::Heal_Implementation(float Amount)
{
	float value = PlayerStatesComponent->Heal(Amount);
	float MaxHealth = PlayerStatesComponent->GetMaxHealth();
	UXWidget_EnemyHeadHP* Widget = Cast<UXWidget_EnemyHeadHP>(CharacterHPWidget->GetWidget());
	if (Widget)
	{
		Widget->HealthBar->SetPercent(value / MaxHealth);
	}
	return value;
}

bool AXClimbCharacter::IsDead_Implementation()
{
	return PlayerStatesComponent->GetIsDeath();
}

bool AXClimbCharacter::IsAttacking_Implementation()
{
	return bAttacking;
}

void AXClimbCharacter::MoveForWard(float value)
{
	if (!bCanMove) return;
	AxisValue = value;
	FRotator Rotation = GetControlRotation();
	Rotation.Roll = 0.0f;
	Rotation.Pitch = 0.0f;
	FVector FVector = UKismetMathLibrary::GetForwardVector(Rotation);
	AddMovementInput(FVector, value, false);
}

void AXClimbCharacter::MoveRight(float value)
{
	if (!bCanMove) return;
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
{	/* {
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 1000.0f;
		ETraceTypeQuery ETType1st = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
		ETraceTypeQuery ETType2nd = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);
		FHitResult HitRes1st;
		bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			Start,
			End,
			20.0f,
			ETType1st,
			false,
			TArray<AActor*>{this},
			EDrawDebugTrace::ForDuration,
			HitRes1st,
			true
		);
		if (bIsHit)
		{
			return;
		}
		else
		{
			FHitResult HitRes2nd;
			bIsHit = UKismetSystemLibrary::SphereTraceSingle(
				GetWorld(),
				Start,
				End,
				20.0f,
				ETType2nd,
				false,
				TArray<AActor*>{this},
				EDrawDebugTrace::ForDuration,
				HitRes2nd,
				true
			);
			if (bIsHit)
			{
				AXAI_Character* AICha = Cast<AXAI_Character>(HitRes2nd.Actor);
				if (AICha && AICha->Implements<UXDamageInterface>())
				{
					IXDamageInterface::Execute_TakeDamage(AICha, DamageInfo);
					UGameplayStatics::ApplyDamage(AICha, 5.0f, nullptr, this, nullptr);
				}
			}
		}
	}*/
	if (Stance != EPlayerStance::EPS_Magic) return;
	bAttacking = true;
	bCanMove = false;
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
	PlayAnimMontage(MagicAttackMontage);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AXClimbCharacter::OnNotifyMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXClimbCharacter::OnAttackMontageEnd);
}

void AXClimbCharacter::DoHeal()
{
	IXDamageInterface::Execute_Heal(this, 10.0f);
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
			//UE_LOG(LogTemp, Warning, TEXT("Montage Valid And Playing, need Delay!"));
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
			//UE_LOG(LogTemp, Warning, TEXT("Montage Valid But not playing!"));
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
			//UE_LOG(LogTemp, Warning, TEXT("No Montage!"));
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

void AXClimbCharacter::MagicStance()
{
	//Show CrossHair CrossHairWID

	Stance = EPlayerStance::EPS_Magic;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	bInAttack = true;
}

void AXClimbCharacter::DefaultStance()
{
	Stance = EPlayerStance::EPS_Default;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	bInAttack = false;
}

void AXClimbCharacter::CallOnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Character On Death"));
	UXWidget_EnemyHeadHP* Widget = Cast<UXWidget_EnemyHeadHP>(CharacterHPWidget->GetWidget());
	if (Widget)
	{
		Widget->HealthBar->SetPercent(0);
	}
	PlayerStatesComponent->SetIsDeath(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DisableInput(GetWorld()->GetFirstPlayerController());
}

void AXClimbCharacter::CallOnBlocked(bool bCanbeParried)
{
	UE_LOG(LogTemp, Warning, TEXT("Character On Blocked"));
}

void AXClimbCharacter::CallOnDamageResponse(EDamageResponse DamageResponse)
{
	float value = PlayerStatesComponent->GetCurHealth();
	float MaxHealth = PlayerStatesComponent->GetMaxHealth();
	UXWidget_EnemyHeadHP* Widget = Cast<UXWidget_EnemyHeadHP>(CharacterHPWidget->GetWidget());
	if (Widget)
	{
		Widget->HealthBar->SetPercent(value / MaxHealth);
	}
	UE_LOG(LogTemp, Warning, TEXT("Character On DamageResponse"));
}

void AXClimbCharacter::OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName(TEXT("Fire")))
	{
		FDamageInfo DamageInfo;
		DamageInfo.Amount = 10.0f;
		DamageInfo.DamageType = EDamageType::EDT_Projectile;
		DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
		DamageInfo.bCanBeBlocked = true;

		FTransform trans;
		trans.SetLocation(GetMesh()->GetSocketLocation(TEXT("hand_rSocket")));
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(
			GetMesh()->GetSocketLocation(TEXT("hand_rSocket")),
			TP_Camera->GetComponentLocation() + TP_Camera->GetForwardVector() * 1000.0f
		);
		trans.SetRotation(Rot.Quaternion());

		CombatComponent->MagicSpell(trans, nullptr, DamageInfo);
	}
}

void AXClimbCharacter::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bAttacking = false;
	bCanMove = true;
}


