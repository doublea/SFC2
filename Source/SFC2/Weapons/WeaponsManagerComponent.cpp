// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponsManagerComponent.h"
#include "PhaserEmitterComponent.h"
#include "ShipPawn.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UWeaponsManagerComponent::UWeaponsManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;

	// ...
}

void UWeaponsManagerComponent::InitializeComponent() {
    Super::InitializeComponent();

    // Create emitters
    for (int idx = 0; idx < WeaponModels->Num(); ++idx) {
        UWeaponEmitterComponent* NewEmitter = nullptr;
        FString Name = TEXT("Weapon");
        Name.AppendInt(idx);
        switch ((*WeaponModels)[idx].Type) {
        case EWeaponType::WT_Phaser:
            NewEmitter = NewObject<UPhaserEmitterComponent>(GetOwner(), FName(*Name));
            CHECK(NewEmitter != nullptr);
            break;
        case EWeaponType::WT_Photon:
            // TODO
            break;
        default:
            CHECKMSG(false, "Unknown weapon type");
        }
        if (NewEmitter != nullptr) {
            NewEmitter->Mesh = Mesh;
            NewEmitter->SocketName = (*WeaponModels)[idx].SocketName;
            NewEmitter->RegisterComponent();
            NewEmitter->Activate();
        }
        // We add NewEmitter even if it's NULL, since we want to maintain the
        // correspondence of indexes between WeaponEmitters and WeaponModels.
        WeaponEmitters.push_back(NewEmitter);
        NewEmitter = nullptr;
    }
}

// Called when the game starts
void UWeaponsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

    Ship = Cast<AShipPawn>(GetOwner());
    CHECKMSG(
         Ship != nullptr, 
        "UWeaponsManagerComponent can only be used by AShipPawn actors.");
}

bool UWeaponsManagerComponent::FireWeapon(int WeaponIdx, AActor* Target) {
    if (Target == nullptr) return false;
    if (WeaponIdx >= WeaponEmitters.size()) return false;
    if (WeaponEmitters[WeaponIdx] == nullptr) return false;
    WeaponEmitters[WeaponIdx]->FireAtTarget(Target);
    return true;
}