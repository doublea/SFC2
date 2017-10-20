// Fill out your copyright notice in the Description page of Project Settings.

#include "PhaserEmitterComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ShipPawn.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Models/DamageTypes.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UPhaserEmitterComponent::UPhaserEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


    struct FConstructorStatics {
        ConstructorHelpers::FObjectFinderOptional<UParticleSystem> PhaserParticle;
        FConstructorStatics() :
            PhaserParticle(TEXT("/Game/Particles/P_Phaser")) {};
    };
    static FConstructorStatics ConstructorStatics;

    FName Name(*(GetName() + TEXT("-PhaserParticleSystem")));
    PhaserParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(Name);
    if (ConstructorStatics.PhaserParticle.Succeeded()) {
        PhaserParticleSystem->Template = ConstructorStatics.PhaserParticle.Get();
    }
    PhaserParticleSystem->bAutoActivate = false;
    PhaserParticleSystem->SetHiddenInGame(false);
}


// Called when the game starts
void UPhaserEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

    PhaserParticleSystem->SetupAttachment(Mesh, SocketName);
    PhaserParticleSystem->RegisterComponentWithWorld(GetWorld());
}


bool UPhaserEmitterComponent::FireAtTarget(AActor* Target) {
    if (Target == nullptr) return false;
    CHECK(PhaserParticleSystem != nullptr);
    PhaserParticleSystem->SetActorParameter(FName(TEXT("BeamTarget")), Target);
    PhaserParticleSystem->ActivateSystem();
    FVector HitFromDirection = GetOwner()->GetActorLocation() - Target->GetActorLocation();
    HitFromDirection.Normalize();
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(GetOwner());
    TraceParams.bTraceComplex = true;
    // This is necessary for UGameplayStatics::FindCollisionUV to work. See UShieldComponent::ShieldCollision.
    TraceParams.bReturnFaceIndex = true;
    TraceParams.TraceTag = FName(TEXT("Phaser trace"));
    FHitResult Hit(ForceInit);
    GetWorld()->LineTraceSingleByChannel(
        Hit, GetOwner()->GetActorLocation(), Target->GetActorLocation(),
        ECC_SFCWeaponTraceChannel, TraceParams);
    if (!Hit.bBlockingHit) {
        DEBUGMSG("Phaser trace didn't hit anything!");
    }
    AShipPawn* OwnShip = Cast<AShipPawn>(GetOwner());
    CHECK(OwnShip);
    UGameplayStatics::ApplyPointDamage(
        Target, 10.0f, HitFromDirection, Hit, OwnShip->GetController(),
        GetOwner(), USFCPhaserDamageType::StaticClass());
    return true;
}

