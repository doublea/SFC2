// Fill out your copyright notice in the Description page of Project Settings.

#include "ShieldComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetRenderingLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UShieldComponent::UShieldComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    struct FConstructorStatics {
		ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldMesh;
        ConstructorHelpers::FObjectFinder<UParticleSystem> ShieldParticle;
		FConstructorStatics()
            : ShieldMesh(TEXT("/Game/Meshes/Shape_Sphere")),
              ShieldParticle(TEXT("/Game/Particles/P_Shield")) {}
	};
	static FConstructorStatics ConstructorStatics;

    ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
    if (ConstructorStatics.ShieldMesh.Succeeded()) {
        ShieldMesh->SetStaticMesh(ConstructorStatics.ShieldMesh.Object);
    }
    ShieldMesh->SetVisibility(false);

    ShieldParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShieldParticle"));
    ShieldParticleSystem->Template = ConstructorStatics.ShieldParticle.Object;
}


// Called when the game starts
void UShieldComponent::BeginPlay()
{
	Super::BeginPlay();

    ShieldMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
    ShieldMesh->SetWorldLocation(GetOwner()->GetActorLocation());
    ShieldMesh->SetWorldRotation(GetOwner()->GetActorRotation());
    ShieldMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    ShieldMesh->SetCollisionResponseToChannel(ECC_SFCWeaponTraceChannel, ECollisionResponse::ECR_Block);
    ShieldMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ShieldMesh->bGenerateOverlapEvents = false;

    ShieldParticleSystem->AttachToComponent(ShieldMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
    ShieldParticleSystem->SetWorldLocation(GetOwner()->GetActorLocation());
}

void UShieldComponent::ShieldCollision(const FHitResult& HitInfo) {
    FRotator ShieldRot = UKismetMathLibrary::FindLookAtRotation(
           ShieldParticleSystem->GetComponentLocation(), HitInfo.ImpactPoint);
    // TODO: Can we put the hit direction in the event and spawn a particle with the correct rotation?
    ShieldParticleSystem->SetVectorParameter(TEXT("ParticleRotation"), FVector(0.0f, 0.0f, (90.0f + ShieldRot.Yaw) / 360.0f));
    ShieldParticleSystem->GenerateParticleEvent(TEXT("ShieldHit"), 0.0f, FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector);
    ShieldParticleSystem->ActivateSystem();
}

