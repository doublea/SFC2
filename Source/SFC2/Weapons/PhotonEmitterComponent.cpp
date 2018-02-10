// Fill out your copyright notice in the Description page of Project Settings.

#include "PhotonEmitterComponent.h"
#include "WeaponActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Engine/Blueprint.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "Models/ShipModel.h"
#include "SFCUtils.h"


UPhotonEmitterComponent::UPhotonEmitterComponent() {
    static ConstructorHelpers::FObjectFinder<UBlueprint> PhotonBPFinder(TEXT("/Game/Blueprints/BP_Photon"));
    static ConstructorHelpers::FObjectFinder<USoundBase> PhotonSoundFinder(TEXT("/Game/Sound/Photon1"));
    if (PhotonBPFinder.Succeeded()) {
        PhotonBlueprint = (UClass*)PhotonBPFinder.Object->GeneratedClass;
    }
    if (PhotonSoundFinder.Succeeded()) {
        PhotonSound = PhotonSoundFinder.Object;
    }
}

bool UPhotonEmitterComponent::FireAtTarget(FWeaponModel WeaponState, AActor* Target) {
    FActorSpawnParameters Params;
    Params.Instigator = Cast<APawn>(GetOwner());
    Params.Owner = GetOwner();
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform Transform = Mesh->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_World);

    AActor* Actor = GetWorld()->SpawnActorAbsolute(PhotonBlueprint, Transform, Params);
    AWeaponActor* WeaponActor = Cast<AWeaponActor>(Actor);
    CHECK(WeaponActor != nullptr);
    TSubclassOf<USFCDamageType> DamageType;
    switch (WeaponState.WeaponMode) {
    case EWeaponMode::WM_Photon_Normal:
        DamageType = USFCNormalPhotonDamageType::StaticClass();
        break;
    case EWeaponMode::WM_Photon_Overload:
        DamageType = USFCOverloadPhotonDamageType::StaticClass();
        break;
    case EWeaponMode::WM_Photon_Proximity:
        DamageType = USFCProximityPhotonDamageType::StaticClass();
        break;
    default:
        CHECKMSG(false, "Invalid photon mode.");
    }
    WeaponActor->Init(GetOwner(), Target, DamageType);
    if (PhotonSound != nullptr) {
        UGameplayStatics::SpawnSoundAttached(PhotonSound, WeaponActor->GetRootComponent());
    }
    return true;
}
