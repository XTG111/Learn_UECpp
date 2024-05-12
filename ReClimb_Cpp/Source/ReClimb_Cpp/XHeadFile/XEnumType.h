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