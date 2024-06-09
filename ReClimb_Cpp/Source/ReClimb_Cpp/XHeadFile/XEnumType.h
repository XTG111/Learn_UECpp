#pragma once

//攀爬类型的枚举
UENUM(BlueprintType)
enum class EClimbingTypes : uint8
{
	ECT_None UMETA(DisplayName = "None"),
	ECT_JumpClimb UMETA(DisplayName = "JumpClimb"),
	ECT_HighClimb UMETA(DisplayName = "HighClimb"),
	ECT_LowClimb UMETA(DisplayName = "LowClimb"),
};

//攀爬状态的枚举
UENUM(BlueprintType)
enum class EClimbingStates : uint8
{
	ECS_None UMETA(DisplayName = "None"),
	ECS_LowVault UMETA(DisplayName = "LowVault"),
	ECS_HighVault UMETA(DisplayName = "HighVault"),
	ECS_HighJumpVault UMETA(DisplayName = "HighJumpVault"),
	ECS_LowClimb UMETA(DisplayName = "LowClimb"),
	ECS_HighClimb UMETA(DisplayName = "HighClimb"),
	ECS_HighJumpClimb UMETA(DisplayName = "HighJumpClimb"),
};

//AISenseEnum
UENUM(BlueprintType)
enum class EAISense : uint8
{
	EASe_None UMETA(DisplayName = "None"),
	EASe_Sight UMETA(DisplayName = "Sight"),
	EASe_Hearing UMETA(DisplayName = "Hearing"),
	EASe_Damaging UMETA(DisplayName = "Damaging")
};

//AIStateEnum
UENUM(BlueprintType)
enum class EAIState : uint8
{
	EASt_Passive UMETA(DisplayName = "Passive"),
	EASt_Attacking UMETA(DisplayName = "Attacking"),
	EASt_Frozen UMETA(DisplayName = "Frozen"),
	EASt_Investingating UMETA(DisplayName = "Investingating"),
	EASt_Dead UMETA(DisplayeName = "Dead")
};

//AIMovementEnum
UENUM(BlueprintType)
enum class EAIMovement : uint8
{
	EAM_Idle UMETA(DisplayName = "Idle"),
	EAM_Walking UMETA(DisplayName = "Walking"),
	EAM_Joggiing UMETA(DisplayName = "Jogging"),
	EAM_Sprinting UMETA(DisplayeName = "Sprinting")
};

//DamageType
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EDT_None UMETA(DisplayName = "None"),
	EDT_Melee UMETA(DisplayName = "Melee"),
	EDT_Projectile UMETA(DisplayName = "Projectile"),
	EDT_Explosion UMETA(DisplayeName = "Explosion"),
	EDT_Environment UMETA(DisplayeName = "Environment")
};

//DamageResponse
UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	EDR_None UMETA(DisplayName = "None"),
	EDR_HitReaction UMETA(DisplayName = "HitReaction"),
	EDR_Stagger UMETA(DisplayName = "Stagger"),
	EDR_Stun UMETA(DisplayeName = "Stun"),
	EDR_KnockBack UMETA(DisplayeName = "KnockBack")
};