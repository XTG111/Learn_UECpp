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
#include "Component/XCombatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Widget/XHUD.h"
#include "Widget/XWidget_CharacterOverShow.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Projectiles/XProjectilesBase.h"
#include "AOE/XAOEBase.h"

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
		PlayerStatesComponent->SetAttackTokenCount(2);
	}
	PIC = PIC == nullptr ? Cast<APlayerController>(this->Controller) : PIC;
	if (PIC)
	{
		XCharacterHUD = XCharacterHUD == nullptr ? Cast<AXHUD>(PIC->GetHUD()) : XCharacterHUD;
		XCharacterHUD->AddCharacterShow();
		UXWidget_CharacterOverShow* Widget = XCharacterHUD->CharacterShowWdg;
		if (Widget)
		{
			Widget->HealthBar->SetPercent(1);
		}
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
	PlayerInputComponent->BindAction("UnArmedStance", IE_Pressed, this, &AXClimbCharacter::ChangeToUnarmed);
	PlayerInputComponent->BindAction("MagicStance", IE_Pressed, this, &AXClimbCharacter::ChangeToMagic);
	PlayerInputComponent->BindAction("MeleeStance", IE_Pressed, this, &AXClimbCharacter::ChangeToMelee);

	PlayerInputComponent->BindAction("StartBlock", IE_Pressed, this, &AXClimbCharacter::RightMousePress);
	PlayerInputComponent->BindAction("EndBlock", IE_Released, this, &AXClimbCharacter::RightMouseRelease);
}

float AXClimbCharacter::GetCurrentHealth_Implementation()
{
	return PlayerStatesComponent->GetCurHealth();
}

float AXClimbCharacter::GetMaxHealth_Implementation()
{
	return PlayerStatesComponent->GetMaxHealth();
}

bool AXClimbCharacter::TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCausor)
{
	return PlayerStatesComponent->TakeDamage(DamageInfo, DamageCausor);
}

float AXClimbCharacter::Heal_Implementation(float Amount)
{
	float value = PlayerStatesComponent->Heal(Amount);
	float MaxHealth = PlayerStatesComponent->GetMaxHealth();
	UXWidget_CharacterOverShow* Widget = XCharacterHUD->CharacterShowWdg;
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

void AXClimbCharacter::SetIsInteruptible_Implementation(bool binter)
{
	PlayerStatesComponent->SetbIsInterruptible(binter);
}

bool AXClimbCharacter::ReserveAttackToken_Implementation(int Amount)
{
	return PlayerStatesComponent->ReserveAttackToken(Amount);
}

void AXClimbCharacter::ReturnAttackToken_Implementation(int Amount)
{
	PlayerStatesComponent->ReturnAttackToken(Amount);
}

int AXClimbCharacter::GetTeamNumber_Implementation()
{
	return TeamNum;
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
{
	switch (Stance)
	{
	case EPlayerStance::EPS_Magic:
		MagicAttack();
		break;
	case EPlayerStance::EPS_Melee:
		MeleeAttack();
		break;
	default:
		break;
	}
}

void AXClimbCharacter::DoHeal()
{
	IXDamageInterface::Execute_Heal(this, 10.0f);
}

void AXClimbCharacter::ChangeToUnarmed()
{
	ChangeStance(EPlayerStance::EPS_Default);
}

void AXClimbCharacter::ChangeToMagic()
{
	ChangeStance(EPlayerStance::EPS_Magic);
}

void AXClimbCharacter::ChangeToMelee()
{
	ChangeStance(EPlayerStance::EPS_Melee);
}

void AXClimbCharacter::RightMousePress()
{
	switch (Stance)
	{
	case EPlayerStance::EPS_Melee:
		StartBlock();
		break;
	default:
		break;
	}
}

void AXClimbCharacter::RightMouseRelease()
{
	switch (Stance)
	{
	case EPlayerStance::EPS_Melee:
		EndBlock();
		break;
	default:
		break;
	}
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
	if (ShieldActor && SwordActor)
	{
		SaveSwordAndShield();
	}
}

void AXClimbCharacter::MeleeStance()
{
	Stance = EPlayerStance::EPS_Melee;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	bInAttack = true;
	SpawnSwordAndShield();
}

void AXClimbCharacter::DefaultStance()
{
	Stance = EPlayerStance::EPS_Default;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	bInAttack = false;
	if (ShieldActor && SwordActor)
	{
		SaveSwordAndShield();
	}
}

void AXClimbCharacter::ChangeStance(EPlayerStance stance)
{
	EPlayerStance OldStance = Stance;
	DefaultStance();
	switch (stance)
	{
	case EPlayerStance::EPS_Magic:
		if (OldStance != EPlayerStance::EPS_Magic)
		{
			MagicStance();
		}
		break;
	case EPlayerStance::EPS_Melee:
		if (OldStance != EPlayerStance::EPS_Melee)
		{
			MeleeStance();
		}
		break;
	default:
		break;
	}
}

void AXClimbCharacter::SpawnSwordAndShield()
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true
	);
	SwordActor = SwordActor != nullptr ? SwordActor : GetWorld()->SpawnActor<AActor>(SwordClass);
	if (SwordActor)
	{
		SwordActor->AttachToComponent(
			GetMesh(),
			AttachmentRules,
			FName(TEXT("WeaponHandR"))
		);
	}
	ShieldActor = ShieldActor != nullptr ? ShieldActor : GetWorld()->SpawnActor<AActor>(ShieldClass);
	if (ShieldActor)
	{
		ShieldActor->AttachToComponent(
			GetMesh(),
			AttachmentRules,
			FName(TEXT("WeaponHandL"))
		);
	}
}

void AXClimbCharacter::SaveSwordAndShield()
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true
	);
	SwordActor->AttachToComponent(
		GetMesh(),
		AttachmentRules,
		FName(TEXT("SwordSaveSocket"))
	);
	ShieldActor->AttachToComponent(
		GetMesh(),
		AttachmentRules,
		FName(TEXT("ShieldSaveSocket"))
	);
}

void AXClimbCharacter::MagicAttack()
{
	if (bAttacking) return;
	bAttacking = true;
	bCanMove = false;
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
	PlayAnimMontage(MagicAttackMontage);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AXClimbCharacter::OnNotifyMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXClimbCharacter::OnAttackMontageEnd);
}

void AXClimbCharacter::MeleeAttack()
{
	if (bAttacking) return;
	bAttacking = true;
	bCanMove = false;
	//当没有在window里面时，直接播放动画
	if (!bInComboWindow)
	{
		GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.Clear();
		PlayAnimMontage(MeleeAttackMontage);
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AXClimbCharacter::OnNotifyMontage);
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.AddDynamic(this, &AXClimbCharacter::OnNotifyMontageEnd);
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXClimbCharacter::OnAttackMontageEnd);
	}
	//如果在 表示在Montage的窗口之间再次按下了左键，可以Combo
	else
	{
		bCanResumeCombo = true;
	}

}

void AXClimbCharacter::AOEDamageForOverlapActor(AActor* actor)
{
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 20.0f;
	DamageInfo.DamageType = EDamageType::EDT_Projectile;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
	DamageInfo.bCanBeBlocked = false;
	if (actor != this && actor->Implements<UXDamageInterface>())
	{
		if (IXDamageInterface::Execute_GetTeamNumber(this) != IXDamageInterface::Execute_GetTeamNumber(actor))
		{
			IXDamageInterface::Execute_TakeDamage(actor, DamageInfo, this);
		}
	}
}

void AXClimbCharacter::StartBlock()
{
	if (bBlocking) return;
	bBlocking = true;

	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.Clear();
	PlayAnimMontage(BlockAttackMontage);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AXClimbCharacter::OnNotifyMontage);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.AddDynamic(this, &AXClimbCharacter::OnNotifyMontageEnd);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXClimbCharacter::OnAttackMontageEnd);
}

void AXClimbCharacter::EndBlock()
{
	bBlocking = false;
	bInParryWindow = false;
	bIsReactingBlock = false;
	PlayerStatesComponent->SetIsBlocking(false);
	StopAnimMontage(BlockAttackMontage);
}

void AXClimbCharacter::ParryAttack(AActor* ParryActor)
{
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 10.0f;
	DamageInfo.DamageType = EDamageType::EDT_Melee;
	DamageInfo.DamageResponse = EDamageResponse::EDR_Stagger;
	DamageInfo.bShouldForceInterrupt = true;//强制打断
	if (ParryActor && ParryActor->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_TakeDamage(ParryActor, DamageInfo, this);
		if (ParryActor && ParryActor->Implements<UXAIInterface>())
		{
			IXAIInterface::Execute_AttackEnd(ParryActor, this);
		}
	}
}

void AXClimbCharacter::CallOnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Character On Death"));
	UXWidget_CharacterOverShow* Widget = XCharacterHUD->CharacterShowWdg;
	if (Widget)
	{
		Widget->HealthBar->SetPercent(0);
	}
	PlayerStatesComponent->SetIsDeath(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DisableInput(GetWorld()->GetFirstPlayerController());
}

void AXClimbCharacter::CallOnBlocked(AActor* DamageCauser, bool bCanbeParried)
{
	UE_LOG(LogTemp, Warning, TEXT("Character On Blocked"));
	bIsReactingBlock = true; // 正在block
	bool bGoParry = bInParryWindow && bCanbeParried;
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	PlayAnimMontage(bGoParry ? ParryReactMontage : BlockReactMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXClimbCharacter::OnBlockMontageEnd);
	//当在反弹窗口，并且可以反弹伤寒
	if (bGoParry)
	{
		ParryAttack(DamageCauser);
	}
}

void AXClimbCharacter::CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor)
{
	float value = PlayerStatesComponent->GetCurHealth();
	float MaxHealth = PlayerStatesComponent->GetMaxHealth();
	UXWidget_CharacterOverShow* Widget = XCharacterHUD->CharacterShowWdg;
	if (Widget)
	{
		Widget->HealthBar->SetPercent(value / MaxHealth);
	}
	PlayAnimMontage(OnHitMontage);
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

		ETraceTypeQuery ETTQ = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);

		FHitResult RealHit;
		bool bIsHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			TP_Camera->GetComponentLocation(),
			TP_Camera->GetComponentLocation() + TP_Camera->GetForwardVector() * 10000.0f,
			ETTQ,
			false,
			TArray<AActor*>{this},
			EDrawDebugTrace::None,
			RealHit,
			true
		);

		FVector TargetLoc = bIsHit ? RealHit.ImpactPoint : TP_Camera->GetComponentLocation() + TP_Camera->GetForwardVector() * 1000.0f;

		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(
			GetMesh()->GetSocketLocation(TEXT("hand_rSocket")),
			TargetLoc
		);
		trans.SetRotation(Rot.Quaternion());
		CombatComponent->MagicSpell(trans, nullptr, DamageInfo, ProjectileEx);
	}

	if (NotifyName == FName(TEXT("SwordAttack")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Sword Attack"));
		FDamageInfo DamageInfo;
		DamageInfo.Amount = 20.0f;
		DamageInfo.DamageType = EDamageType::EDT_Projectile;
		DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
		DamageInfo.bCanBeBlocked = false;
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 600.0f;
		CombatComponent->SwordRadius = 20.0f;
		CombatComponent->SwordAttack(Start, End, DamageInfo, this);
	}

	if (NotifyName == FName(TEXT("AOESlash")))
	{
		FTransform SpawnTrans = FTransform();
		SpawnTrans.SetLocation(GetActorLocation());
		ComboAOEActor = GetWorld()->SpawnActor<AXAOEBase>(ComboAOEClass, SpawnTrans);
		ComboAOEActor->SphereRadius = 300.0f;
		ComboAOEActor->OnAOEOverlapActor.AddDynamic(this, &AXClimbCharacter::AOEDamageForOverlapActor);
		ComboAOEActor->Trigger();
	}
	if (NotifyName == FName(TEXT("ComboAttackWindow")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin Window bCanResumeCombo: %d"), bCanResumeCombo);
		bInComboWindow = true;
		//使得可以继续攻击
		bAttacking = false;
		bCanMove = true;
		//重置Combo统计
		bCanResumeCombo = false;
	}

	if (NotifyName == FName(TEXT("Block")))
	{
		PlayerStatesComponent->SetIsBlocking(true);

	}
	if (NotifyName == FName(TEXT("ParryWindow")))
	{
		bInParryWindow = true;
	}
}

void AXClimbCharacter::OnNotifyMontageEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName(TEXT("ComboAttackWindow")))
	{
		UE_LOG(LogTemp, Warning, TEXT("End Window bCanResumeCombo: %d"), bCanResumeCombo);
		bInComboWindow = false;
		//如果bCanResumeCombo = false 说明无法combo stop
		if (!bCanResumeCombo)
		{
			StopAnimMontage(MeleeAttackMontage);
		}
	}
	if (NotifyName == FName(TEXT("ParryWindow")))
	{
		bInParryWindow = false;
	}
}

void AXClimbCharacter::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bAttacking = false;
	bCanMove = true;
	bInComboWindow = false;
	bCanResumeCombo = false;
	if (!bInterrupted)
	{
		EndBlock();
	}
	if (bInterrupted && !bIsReactingBlock)
	{
		//避免2个敌人同时攻击导致只能阻挡一个
		EndBlock();
	}
}
void AXClimbCharacter::OnBlockMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	EndBlock();
}



