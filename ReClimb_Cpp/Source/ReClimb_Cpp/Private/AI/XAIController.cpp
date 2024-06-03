// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


AXAIController::AXAIController()
{
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/BluePrint/AI_CPP/AIControl/BHT_XAIBase.BHT_XAIBase'"));
	//if (BTObject.Succeeded())
	//{
	//	BehaviorTree = BTObject.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UBlackboard> BBObject(TEXT("BlackboardData'/Game/BluePrint/AI/AIControl/BB_AIBase.BB_AIBase'"));
	//if (BBObject.Succeeded())
	//{
	//	BlackboardData = BBObject.Object;
	//}

	//添加AIPerceptionComponent组件
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// 创建并配置视觉感知
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(5.0f);

	//配置听觉
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1200.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->SetMaxAge(5.0f);

	//配置受击
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(5.0f);

	// 将配置添加到 AIPerception 组件
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	//绑定
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AXAIController::PerceptionUpdated);

}

void AXAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->Activate();
	}
}

void AXAIController::SetStateAsPassive()
{
	ClearFocus(EAIFocusPriority::Default);
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Passive));
}

void AXAIController::SetStateAsInvestigatinig(const FVector& Location)
{
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Investingating));
	Blackboard->SetValueAsVector(PointOfInterestKeyName, Location);
}

void AXAIController::SetStateAsAttacking(AActor* AttackTarget)
{
	Blackboard->SetValueAsObject(AttackTargetKeyName, AttackTarget);
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Attacking));
}

void AXAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BehaviorTree);
	SetStateAsPassive();
}


void AXAIController::CanSenseActor(AActor* Actor, EAISense type, FAIStimulus& Simulus)
{
	FActorPerceptionBlueprintInfo ActorInfo;
	AIPerceptionComponent->GetActorsPerception(Actor, ActorInfo);
	for (auto& it : ActorInfo.LastSensedStimuli)
	{
		UAISenseConfig* Res = AIPerceptionComponent->GetSenseConfig(it.Type);
		//当前获取ActorInfo的Sense和type是否相等
		UAISenseConfig* Ref = nullptr;
		switch (type)
		{
		case EAISense::EASe_None:
			break;
		case EAISense::EASe_Sight:
			Ref = SightConfig;
			break;
		case EAISense::EASe_Hearing:
			Ref = HearingConfig;
			break;
		case EAISense::EASe_Damaging:
			Ref = DamageConfig;
			break;
		default:
			break;
		}
		if (Res == Ref)
		{
			Simulus = it;
		}
	}
}

EAIState AXAIController::GetCurrentState()
{
	return static_cast<EAIState>(Blackboard->GetValueAsEnum(AIStateKeyName));
}

void AXAIController::HandleSenseSound(const FVector& Location)
{
	EAIState curState = GetCurrentState();
	switch (curState)
	{
	case EAIState::EASt_Passive:
		SetStateAsInvestigatinig(Location);
		break;
	case EAIState::EASt_Attacking:
		break;
	case EAIState::EASt_Frozen:
		break;
	case EAIState::EASt_Investingating:
		SetStateAsInvestigatinig(Location);
		break;
	case EAIState::EASt_Dead:
		break;
	default:
		break;
	}
}

void AXAIController::HandleSenseSight(AActor* Actor)
{
	ACharacter* temp = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	bool check = temp == Cast<ACharacter>(Actor);
	EAIState curState = GetCurrentState();
	switch (curState)
	{
	case EAIState::EASt_Passive:
		if (check)
		{
			SetStateAsAttacking(Actor);
		}
		break;
	case EAIState::EASt_Attacking:
		break;
	case EAIState::EASt_Frozen:
		break;
	case EAIState::EASt_Investingating:
		if (check)
		{
			SetStateAsAttacking(Actor);
		}
		break;
	case EAIState::EASt_Dead:
		break;
	default:
		break;
	}
}

void AXAIController::HandleSenseDamage(AActor* Actor)
{
	EAIState curState = GetCurrentState();
	switch (curState)
	{
	case EAIState::EASt_Passive:
		SetStateAsAttacking(Actor);
		break;
	case EAIState::EASt_Attacking:
		break;
	case EAIState::EASt_Frozen:
		break;
	case EAIState::EASt_Investingating:
		SetStateAsAttacking(Actor);
		break;
	case EAIState::EASt_Dead:
		break;
	default:
		break;
	}
}

void AXAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (auto& it : UpdatedActors)
	{
		FAIStimulus StimulusSight;
		CanSenseActor(it, EAISense::EASe_Sight, StimulusSight);

		FAIStimulus StimulusSound;
		CanSenseActor(it, EAISense::EASe_Hearing, StimulusSound);

		FAIStimulus StimulusDamage;
		CanSenseActor(it, EAISense::EASe_Damaging, StimulusDamage);

		if (StimulusSight.WasSuccessfullySensed())
		{
			HandleSenseSight(it);
		}


		if (StimulusSound.WasSuccessfullySensed())
		{
			HandleSenseSound(StimulusSound.StimulusLocation);
		}


		if (StimulusDamage.WasSuccessfullySensed())
		{
			HandleSenseDamage(it);
		}

		if (!StimulusSight.WasSuccessfullySensed() && !StimulusSound.WasSuccessfullySensed() && !StimulusDamage.WasSuccessfullySensed())
		{
			SetStateAsPassive();
		}
	}
}
