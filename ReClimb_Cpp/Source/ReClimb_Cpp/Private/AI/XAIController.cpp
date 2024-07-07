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
	//添加AIPerceptionComponent组件
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// 创建并配置视觉感知
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(20.0f);

	//配置听觉
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1200.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->SetMaxAge(20.0f);

	//配置受击
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(20.0f);

	// 将配置添加到 AIPerception 组件
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	//绑定
	//AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AXAIController::PerceptionUpdated);

}

void AXAIController::BeginPlay()
{
	Super::BeginPlay();
	ControlledActor = Cast<AActor>(GetPawn());
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

void AXAIController::SetStateAsInvestigatinig(FVector Location)
{
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Investingating));
	Blackboard->SetValueAsVector(PointOfInterestKeyName, Location);
}

void AXAIController::SetStateAsSeeking(FVector Location)
{
	UE_LOG(LogTemp, Warning, TEXT("Seeking State"));
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIState::EASt_Seeking));
	Blackboard->SetValueAsVector(PointOfInterestKeyName, Location);
	GetWorldTimerManager().ClearTimer(SeekingBeginTimer);
}

void AXAIController::SetStateAsAttacking(AActor* AttackTarget, bool bUseLastTarget)
{
	//UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	AActor* NewActor = (bUseLastTarget && AttackTargetActor) ? AttackTargetActor : AttackTarget;
	if (NewActor)
	{
		if (NewActor && NewActor->Implements<UXDamageInterface>())
		{
			//判断角色是否已经死亡
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

	GetWorldTimerManager().SetTimer(CheckForgottenTimer, this, &AXAIController::ForgottenSeeActor, 0.5f, true);
}

void AXAIController::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(CheckForgottenTimer);
}

void AXAIController::ForgottenSeeActor()
{
	TArray<AActor*> NowSeeActor;
	AIPerceptionComponent->GetKnownPerceivedActors(UAISenseConfig_Sight::StaticClass(), NowSeeActor);
	//如果两个数组中的个数不一样那么说明有角色消失了
	int NowLen = NowSeeActor.Num();
	int LastLen = AlreadySeeActors.Num();
	if (NowLen != LastLen)
	{
		for (int i = 0; i < AlreadySeeActors.Num(); i++)
		{
			//说明当前actor 被遗忘了
			if (!NowSeeActor.Contains(AlreadySeeActors[i]))
			{
				UE_LOG(LogTemp, Warning, TEXT("Forgotten"));
				if (AlreadySeeActors[i] && AlreadySeeActors[i]->Implements<UXDamageInterface>())
				{
					if (!IXDamageInterface::Execute_IsDead(AlreadySeeActors[i]))
					{
						HandleForgetSeeActor(AlreadySeeActors[i]);
					}
				}
			}
		}
	}
}

void AXAIController::SeekingActor(AActor* Actor)
{
	GetWorldTimerManager().ClearTimer(SeekingBeginTimer);
	FTimerDelegate SeekingBeginDelegate;
	FVector Loc = Actor->GetActorLocation();
	SeekingBeginDelegate.BindUObject(this, &AXAIController::SetStateAsSeeking, Loc);
	GetWorldTimerManager().SetTimer(SeekingBeginTimer, SeekingBeginDelegate, 2.0f, false);
}

bool AXAIController::CanSenseActor(AActor* Actor, EAISense type, FAIStimulus& Simulus)
{
	FActorPerceptionBlueprintInfo ActorInfo;
	AIPerceptionComponent->GetActorsPerception(Actor, ActorInfo);
	UAIPerceptionSystem* CurAIPerSys = UAIPerceptionSystem::GetCurrent(GetWorld());
	for (auto& it : ActorInfo.LastSensedStimuli)
	{
		TSubclassOf<UAISense> Res = CurAIPerSys->GetSenseClassForStimulus(GetWorld(), it);

		//当前获取ActorInfo的Sense和type是否相等
		TSubclassOf<UAISense> Ref;
		switch (type)
		{
		case EAISense::EASe_None:
			break;
		case EAISense::EASe_Sight:
			Ref = UAISense_Sight::StaticClass();
			break;
		case EAISense::EASe_Hearing:
			Ref = UAISense_Hearing::StaticClass();
			break;
		case EAISense::EASe_Damaging:
			Ref = UAISense_Damage::StaticClass();
			break;
		default:
			break;
		}
		if (Res == Ref)
		{
			Simulus = it;
			return it.WasSuccessfullySensed();
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
	case EAIState::EASt_Seeking:
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
	//将看到的Actor加入到数组中
	UE_LOG(LogTemp, Warning, TEXT("SeeActor"));
	AlreadySeeActors.AddUnique(Actor);
	bool check = !CheckInSameTeam(Actor);
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
		if (Actor == AttackTargetActor)
		{
			GetWorldTimerManager().ClearTimer(SeekingBeginTimer);
		}
		break;
	case EAIState::EASt_Frozen:
		break;
	case EAIState::EASt_Investingating:
		if (check)
		{
			SetStateAsAttacking(Actor, false);
		}
		break;
	case EAIState::EASt_Seeking:
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
	bool check = !CheckInSameTeam(Actor);
	if (!check)
	{
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
		case EAIState::EASt_Seeking:
			SetStateAsAttacking(Actor, false);
			break;
		case EAIState::EASt_Dead:
			break;
		default:
			break;
		}
	}
}

void AXAIController::HandleForgetSeeActor(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("RemoveActor"));
	AlreadySeeActors.Remove(Actor);
	if (Actor == AttackTargetActor)
	{
		SetStateAsPassive();
	}
}

void AXAIController::HandleLostSight(AActor* Actor)
{
	if (Actor == AttackTargetActor)
	{
		EAIState curState = GetCurrentState();
		switch (curState)
		{
		case EAIState::EASt_Passive:
			break;
		case EAIState::EASt_Attacking:
			SeekingActor(Actor);
			//SetStateAsSeeking(Actor->GetActorLocation());
			break;
		case EAIState::EASt_Frozen:
			SeekingActor(Actor);
			//SetStateAsSeeking(Actor->GetActorLocation());
			break;
		case EAIState::EASt_Investingating:
			SeekingActor(Actor);
			//SetStateAsSeeking(Actor->GetActorLocation());
			break;
		case EAIState::EASt_Seeking:
			break;
		case EAIState::EASt_Dead:
			break;
		default:
			break;
		}
	}
}

void AXAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	;
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
		else
		{
			HandleLostSight(it);
		}
		if (bSound)
		{
			HandleSenseSound(StimulusSound.StimulusLocation);
		}
		if (bDamage)
		{
			HandleSenseDamage(it);
		}
	}
}

bool AXAIController::CheckInSameTeam(AActor* OtherActor)
{
	int OtherActorTeam, SelfActorTeam;
	if (OtherActor && OtherActor->Implements<UXDamageInterface>())
	{
		OtherActorTeam = IXDamageInterface::Execute_GetTeamNumber(OtherActor);
	}

	if (ControlledActor && ControlledActor->Implements<UXDamageInterface>())
	{
		SelfActorTeam = IXDamageInterface::Execute_GetTeamNumber(ControlledActor);
	}

	if (OtherActorTeam == SelfActorTeam) return true;
	return false;
}
