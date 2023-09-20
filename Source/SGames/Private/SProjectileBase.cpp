// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Sound/SoundCue.h"
#include "../SGames.h"

ASProjectileBase::ASProjectileBase()
{
	//碰撞
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	//特效
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	//声音
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	//抛物线组件
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->InitialSpeed = 1000;
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOGFMT(LogGame, Warning, "OnActorHit:InstigatorActorName:{name},OtherActorName:{name}", GetNameSafe(GetInstigator()), GetNameSafe(OtherActor));
	//LogOnScreen(this, FString::Printf(TEXT("OnActorHit:InstigatorActorName:{%s},OtherActorName:{%s}"), *GetNameSafe(GetInstigator()), *GetNameSafe(OtherActor)), FColor::Yellow);
	Explode();
}

void ASProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

		Destroy();
	}
}

