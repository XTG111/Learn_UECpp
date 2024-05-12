// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XPlayerStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RECLIMB_CPP_API UXPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXPlayerStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Health = 100.0f;
	float Stamina = 100.0f;

public:
	inline float GetHealth() { return Health; }
	inline float GetStamina() { return Stamina; }
	inline void SetHealth(float value) { Health = value; }
	inline void SetStamina(float value) { Stamina = value; }
};
