// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XNavLinkProxy_EnemyAI.h"
#include "AIInterface/XAIInterface.h"
#include "AI/XAIController.h"

void AXNavLinkProxy_EnemyAI::BeginPlay()
{
	OnSmartLinkReached.AddDynamic(this, &AXNavLinkProxy_EnemyAI::SmartLinkReached);
}

void AXNavLinkProxy_EnemyAI::SmartLinkReached(AActor* Agent, const FVector& Destination)
{

	if (Agent && Agent->Implements<UXAIInterface>())
	{

		UE_LOG(LogTemp, Warning, TEXT("Reached"));
		AXAIController* PIC = Cast<AXAIController>(Agent->GetInstigatorController());
		PIC->ClearFocus(EAIFocusPriority::LastFocusPriority);
		IXAIInterface::Execute_JumpToLoc(Agent, const_cast<FVector&>(Destination));
	}
}
