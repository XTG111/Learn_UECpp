// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/XProjectilesBase.h"
#include "XProjectiles_Dark.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API AXProjectiles_Dark : public AXProjectilesBase
{
	GENERATED_BODY()
public:
	AXProjectiles_Dark();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		class UParticleSystemComponent* ParticleSystem;

};
