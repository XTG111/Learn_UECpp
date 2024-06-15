// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/XProjectiles_Dark.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AXProjectiles_Dark::AXProjectiles_Dark()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;
	if (TargetActor)
	{
		if (bIsHoming)
		{
			ProjectileMovement->bIsHomingProjectile = true;
			ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
			ProjectileMovement->HomingAccelerationMagnitude = 200.0f;
		}
	}
}

#if WITH_EDITOR
void AXProjectiles_Dark::PostEditChangeProperty(FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);

	FName PropertyName = Event.Property != nullptr ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AXProjectiles_Dark, Speed))
	{
		if (ProjectileMovement)
		{
			ProjectileMovement->MaxSpeed = Speed;
			ProjectileMovement->InitialSpeed = Speed;
		}
	}
}
#endif
