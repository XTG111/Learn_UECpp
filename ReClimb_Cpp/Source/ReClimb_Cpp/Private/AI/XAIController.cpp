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
#include "AI/XAI_Character.h"


AXAIController::AXAIController()
{
	//���AIPerceptionComponent���
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// �����������Ӿ���֪
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(5.0f);

	//��������
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1200.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->SetMaxAge(5.0f);

	//�����ܻ�
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(5.0f);

	// ��������ӵ� AIPerception ���
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	//��
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
	ClearFocus(EAIFocusPriority::LastFocusPriority);
	AttackTargetActor = nullptr;
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Passive));
}

void AXAIController::SetStateAsDead()
{
	ClearFocus(EAIFocusPriority::LastFocusPriority);
	AttackTargetActor = nullptr;
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Dead));
}

void AXAIController::SetStateAsInvestigatinig(const FVector& Location)
{
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Investingating));
	Blackboard->SetValueAsVector(PointOfInterestKeyName, Location);
}

void AXAIController::SetStateAsAttacking(AActor* AttackTarget, bool bUseLastTarget)
{
	//UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	AActor* NewActor = (bUseLastTarget && AttackTargetActor) ? AttackTargetActor : AttackTarget;
	if (NewActor)
	{
		if (NewActor && NewActor->Implements<UXDamageInterface>())
		{
			//�жϽ�ɫ�Ƿ��Ѿ�����
			if (IXDamageInterface::Execute_IsDead(NewActor))
			{
				SetStateAsPassive();
			}
			else
			{
				Blackboard->SetValueAsObject(AttackTargetKeyName, NewActor);
				Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Attacking));
				AttackTargetActor = NewActor;
			}
		}
	}
	else
	{
		SetStateAsPassive();
	}
}

void AXAIController::SetStateAsFrozen()
{
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Frozen));
}

void AXAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AXAI_Character* ControlledPawn = Cast<AXAI_Character>(InPawn);
	BehaviorTree = ControlledPawn->BehaviorTree;
	RunBehaviorTree(BehaviorTree);
	SetStateAsPassive();
	float AttackRadius;
	float DefendRadius;

	if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
	{
		IXAIInterface::Execute_GetIdealRange(ControlledPawn, AttackRadius, DefendRadius);
		Blackboard->SetValueAsFloat(AttackRangeKeyName, AttackRadius);
		Blackboard->SetValueAsFloat(DefendRangeKeyName, DefendRadius);
	}

}


bool AXAIController::CanSenseActor(AActor* Actor, EAISense type, FAIStimulus& Simulus)
{
	FActorPerceptionBlueprintInfo ActorInfo;
	AIPerceptionComponent->GetActorsPerception(Actor, ActorInfo);

	for (auto& it : ActorInfo.LastSensedStimuli)
	{
		UAISenseConfig* Res = AIPerceptionComponent->GetSenseConfig(it.Type);
		//��ǰ��ȡActorInfo��Sense��type�Ƿ����
		UAISenseConfig* Ref = nullptr;
		switch (type)
		{
		case EAISense::EASe_None:
			break;
		case EAISense::EASe_Sight:
			UE_LOG(LogTemp, Warning, TEXT("EASe_Sight"));
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
			UE_LOG(LogTemp, Warning, TEXT("Res == Ref"));
			Simulus = it;
			return true;
		}
	}
	return false;
}

EAIState AXAIController::GetCurrentState()
{
	return static_cast<EAIState>(Blackboard->GetValueAsEnum(AIStateKeyName));
}

void AXAIController::HandleSenseSound(const FVector& Location)
{
	//UE_LOG(LogTemp, Warning, TEXT("HandleSenseSound"));
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
	//UE_LOG(LogTemp, Warning, TEXT("HandleSenseSight"));
	ACharacter* temp = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	bool check = true;//temp == Cast<ACharacter>(Actor);
	EAIState curState = GetCurrentState();
	switch (curState)
	{
	case EAIState::EASt_Passive:
		if (check)
		{
			SetStateAsAttacking(Actor, false);
		}
		break;
	case EAIState::EASt_Attacking:
		break;
	case EAIState::EASt_Frozen:
		break;
	case EAIState::EASt_Investingating:
		if (check)
		{
			SetStateAsAttacking(Actor, false);
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
	//UE_LOG(LogTemp, Warning, TEXT("HandleSenseDamage"));
	EAIState curState = GetCurrentState();
	switch (curState)
	{
	case EAIState::EASt_Passive:
		SetStateAsAttacking(Actor, false);
		break;
	case EAIState::EASt_Attacking:
		break;
	case EAIState::EASt_Frozen:
		break;
	case EAIState::EASt_Investingating:
		SetStateAsAttacking(Actor, false);
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
		bool bSight = CanSenseActor(it, EAISense::EASe_Sight, StimulusSight);

		FAIStimulus StimulusSound;
		bool bSound = CanSenseActor(it, EAISense::EASe_Hearing, StimulusSound);

		FAIStimulus StimulusDamage;
		bool bDamage = CanSenseActor(it, EAISense::EASe_Damaging, StimulusDamage);

		if (bSight)
		{
			HandleSenseSight(it);
		}

		if (bSound)
		{
			HandleSenseSound(StimulusSound.StimulusLocation);
		}


		if (bDamage)
		{
			HandleSenseDamage(it);
		}

		//if (!StimulusSight.WasSuccessfullySensed() && !StimulusSound.WasSuccessfullySensed() && !StimulusDamage.WasSuccessfullySensed())
		//{
		//	SetStateAsPassive();
		//}
	}
}
