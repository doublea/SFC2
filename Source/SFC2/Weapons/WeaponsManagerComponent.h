#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponEmitterComponent.h"
#include <vector>
#include "SFCPoweredSystem.h"
#include "Models/ShipModel.h"
#include "WeaponsManagerComponent.generated.h"

class WeaponState : public ISFCPowerConsumer {
public:

    WeaponState(EWeaponType WeaponType, FSystemCharge SystemCharge, FSystemDamage SystemDamage);
    virtual float ConsumePower(SystemPriority Priority, float AvailablePower, float TurnFraction) override;

private:
    EWeaponType Type;
    FSystemCharge Charge;
    SystemPriority PowerPriority;
    FSystemDamage Damage;
};

/**
 * This class manages all weapon operations for a ShipPawn.
 * It contains descriptors, state, and visual components representing all
 * weapons on a ship.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UWeaponsManagerComponent : public UActorComponent, public ISFCPoweredSystem
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UWeaponsManagerComponent();

    TArray<FWeaponModel>* WeaponModels;

    std::vector<UWeaponEmitterComponent*> WeaponEmitters;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* Mesh;

    UFUNCTION(BlueprintCallable)
    bool FireWeapon(int WeaponIdx, AActor* Target);

    // From ISFCPoweredSystem
    virtual std::vector<ISFCPowerConsumer*> GetPowerConsumers() override;

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

private:
    class AShipPawn* Ship;
	
};
