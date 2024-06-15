// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XProjectilesBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileImpact, AActor*, OtherActor, const FHitResult&, HitRes);

UCLASS()
class RECLIMB_CPP_API AXProjectilesBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXProjectilesBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpilse, const FHitResult& Hit);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

public:
	//Åö×²×é¼þ
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereCollision;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
		class USoundCue* ImpactSound;

	UPROPERTY()
		AActor* TargetActor;

public:
	UPROPERTY(EditAnywhere)
		float Speed = 0.0f;
	UPROPERTY(EditAnywhere)
		float Gravity = 0.0f;
	UPROPERTY(EditAnywhere)
		bool bIsHoming = false;
public:
	FOnProjectileImpact OnProjectileImpact;
};
