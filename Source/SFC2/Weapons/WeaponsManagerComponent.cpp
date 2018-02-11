// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponsManagerComponent.h"
#include "PhaserEmitterComponent.h"
#include "PhotonEmitterComponent.h"
#include "ShipPawn.h"
#include "SFCUtils.h"

float WeaponState::ConsumePower(SystemPriority Priority, float AvailablePower, float TurnFraction) {
    if (Model->Damage.IsDestroyed()) {
        return 0.0f;
    }
    return Model->Charge.AddCharge(AvailablePower, TurnFraction);
}

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
        FWeaponModel* model = &(*WeaponModels)[idx];
        UWeaponEmitterComponent* NewEmitter = nullptr;
        FString Name = TEXT("Weapon");
        Name.AppendInt(idx);
        switch (model->Type) {
        case EWeaponType::WT_Phaser:
            NewEmitter = NewObject<UPhaserEmitterComponent>(GetOwner(), FName(*Name));
            break;
        case EWeaponType::WT_Photon:
            NewEmitter = NewObject<UPhotonEmitterComponent>(GetOwner(), FName(*Name));
            break;
        default:
            CHECKMSG(false, "Unknown weapon type");
        }
        CHECK(NewEmitter != nullptr);
        NewEmitter->Mesh = Mesh;
        NewEmitter->SocketName = (*WeaponModels)[idx].SocketName;
        NewEmitter->RegisterComponent();
        NewEmitter->Activate();
        auto state = new WeaponState(model, NewEmitter);
        WeaponStates.push_back(state);
        PowerConsumers.push_back(state);
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
    if (WeaponIdx >= WeaponStates.size()) return false;
    if (WeaponStates[WeaponIdx] == nullptr) return false;

    WeaponState* state = WeaponStates[WeaponIdx];

    if (state->Model->Damage.IsDestroyed()) return false;
    if (!state->Model->Charge.IsCharged()) {
        DEBUGMSG("Weapon not charged.");
        return false;
    }

    if (state->Emitter->FireAtTarget(*state->Model, Target)) {
        state->Model->Charge.Discharge();
        return true;
    }
    return false;
}

std::vector<ISFCPowerConsumer*> UWeaponsManagerComponent::GetPowerConsumers() {
    return PowerConsumers;
}