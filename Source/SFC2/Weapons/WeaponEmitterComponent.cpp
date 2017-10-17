// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponEmitterComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ShipPawn.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


// Sets default values for this component's properties
UWeaponEmitterComponent::UWeaponEmitterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    struct FConstructorStatics {
        ConstructorHelpers::FObjectFinder<UParticleSystem> PhaserParticle;
        FConstructorStatics() :
            PhaserParticle(TEXT("/Game/Particles/P_Phaser")) {};
    };
    static FConstructorStatics ConstructorStatics;

    PhaserParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PhaserParticleSystem"));
    if (ConstructorStatics.PhaserParticle.Succeeded()) {
        PhaserParticleSystem->Template = ConstructorStatics.PhaserParticle.Object;
    }
    PhaserParticleSystem->bAutoActivate = false;
    PhaserParticleSystem->SetHiddenInGame(false);
}


// Called when the game starts
void UWeaponEmitterComponent::BeginPlay()
{
	Super::BeginPlay();
    PhaserParticleSystem->AttachToComponent(
        Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
}

void UWeaponEmitterComponent::FireAtTarget(AActor* target) {
    PhaserParticleSystem->SetActorParameter(FName(TEXT("BeamTarget")), target);
    PhaserParticleSystem->ActivateSystem();
}