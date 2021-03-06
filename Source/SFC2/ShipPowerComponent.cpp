// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPowerComponent.h"
#include "SFCGameState.h"
#include "Models/ShipModel.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UShipPowerComponent::UShipPowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    CurrentExcessPower = 0.0f;
}

void UShipPowerComponent::Init(const FPowerSystemModel Model, std::vector<ISFCPoweredSystem*> PoweredSystems) {
    PowerSystemModel = Model;
    Systems = PoweredSystems;
    CurrentPower = PowerSystemModel.MaxPower;
}


void UShipPowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    ASFCGameState* GameState = Cast<ASFCGameState>(GetWorld()->GetGameState());

    // This all needs rethinking.
    float TurnFraction = DeltaTime / GameState->GameSpeed;
    float AvailablePower = CurrentPower * TurnFraction;
    // TODO: Dynamically sorted priorities.
    for (uint8 Priority = 0; Priority <= ISFCPoweredSystem::PRIORITY_LAST; Priority++) {
        for (ISFCPoweredSystem* system : Systems) {
            for (ISFCPowerConsumer* consumer : system->GetPowerConsumers()) {
                AvailablePower -= consumer->ConsumePower(Priority, AvailablePower, TurnFraction);
                if (AvailablePower < 0.0f) {
                    // This should never happen?
                    AvailablePower = 0.0f;
                }
            }
        }
    }
    CurrentExcessPower = AvailablePower / TurnFraction;
	// ...
}
