// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XSwordBase.generated.h"

UCLASS()
class RECLIMB_CPP_API AXSwordBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXSwordBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class USceneComponent* RootSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* SwordMeshComp;

public:
	// 静态网格资源的路径
	FString StaticMeshPath;

};
