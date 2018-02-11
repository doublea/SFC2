// Fill out your copyright notice in the Description page of Project Settings.

#include "PhaserEmitterComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ShipPawn.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/ParticleEmitterInstances.h"
#include "Runtime/Engine/Public/ParticleEmitterInstances.h"
#include "Models/DamageTypes.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UPhaserEmitterComponent::UPhaserEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;


    struct FConstructorStatics {
        ConstructorHelpers::FObjectFinder<UParticleSystem> PhaserParticle;
        FConstructorStatics() :
            PhaserParticle(TEXT("/Game/Particles/P_Phaser")) {};
    };
    static FConstructorStatics ConstructorStatics;

    FName Name(*(GetName() + TEXT("-PhaserParticleSystem")));
    PhaserParticle = ConstructorStatics.PhaserParticle.Object;
    PhaserParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(Name);
    PhaserParticleSystem->bAutoActivate = false;
}


// Called when the game starts
void UPhaserEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

    PhaserParticleSystem->SetupAttachment(Mesh, SocketName);
    PhaserParticleSystem->RegisterComponentWithWorld(GetWorld());
    PhaserParticleSystem->SetTemplate(PhaserParticle);
}

bool UPhaserEmitterComponent::FireAtTarget(const FWeaponModel& WeaponState, AActor* Target) {
    if (Target == nullptr) return false;
    CurrentTarget = Target;
    CHECK(PhaserParticleSystem != nullptr);
    AShipPawn* OwnShip = Cast<AShipPawn>(GetOwner());
    CHECK(OwnShip);

    // We should use Hit.ImpactPoint, but for some reason I can't get SetBeamTargetPoint to work.
    PhaserParticleSystem->ActivateSystem();
    FVector HitFromDirection = GetOwner()->GetActorLocation() - Target->GetActorLocation();
    HitFromDirection.Normalize();
    FHitResult Hit(ForceInit);
    UGameplayStatics::ApplyPointDamage(
        Target, 10.0f, HitFromDirection, Hit, OwnShip->GetController(),
        GetOwner(), USFCPhaserDamageType::StaticClass());
    return true;
}

void UPhaserEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {
    if (CurrentTarget != nullptr && PhaserParticleSystem->IsActive() && !PhaserParticleSystem->HasCompleted()) {

        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(GetOwner());
        TraceParams.TraceTag = FName(TEXT("Phaser trace"));
        FHitResult Hit(ForceInit);
        GetWorld()->LineTraceSingleByChannel(
            Hit, PhaserParticleSystem->GetComponentLocation(), CurrentTarget->GetActorLocation(),
            ECC_SFCWeaponTraceChannel, TraceParams);
        if (!Hit.bBlockingHit) {
            DEBUGMSG("Phaser trace didn't hit anything!");
        }
        PhaserParticleSystem->SetBeamTargetPoint(0, Hit.ImpactPoint, 0);
    }

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
