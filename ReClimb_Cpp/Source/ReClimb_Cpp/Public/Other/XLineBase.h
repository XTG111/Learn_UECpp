// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XLineBase.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class RECLIMB_CPP_API AXLineBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXLineBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USplineComponent* PatrolRoute;

	//Cur Point Index
	int PatrolIndex = 0;
	//change base
	int Direction = 0;
public:
	FVector GetSplinePointAsWorldPosition();
	void IncrementPatrolRoute();

};
