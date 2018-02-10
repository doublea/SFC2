// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "SFCUtils.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Collision->SetCollisionResponseToChannel(ECC_SFCWeaponTraceChannel, ECollisionResponse::ECR_Block);
    RootComponent = Collision;
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AWeaponActor::Init(AActor * Source, AActor * Target, TSubclassOf<USFCDamageType> DamageType) {
    SourceActor = Source;
    TargetActor = Target;
    this->DamageType = DamageType;

    ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
}
