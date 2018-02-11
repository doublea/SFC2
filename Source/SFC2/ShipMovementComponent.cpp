// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipMovementComponent.h"
#include "SFCUtils.h"
#include <vector>

static const float SPEED_TO_ENGINE_MOVEMENT = 25.0f;

// Sets default values for this component's properties
UShipMovementComponent::UShipMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
    CurrentSpeed = 0.0f;
}

void UShipMovementComponent::Init(FMovementModel Model) {
    this->Model = Model;
}

void UShipMovementComponent::SetSpeedFraction(float speedFraction) {
	CHECK(0.0f <= speedFraction && speedFraction <= 1.0f);
	CurrentSpeed = Model.MaxSpeed * speedFraction;
}

void UShipMovementComponent::SpeedUp() {
	CurrentSpeed = std::min(Model.MaxSpeed, CurrentSpeed + Model.MaxSpeed / 20.0f);
}

void UShipMovementComponent::SpeedDown() {
	CurrentSpeed = std::max(0.0f, CurrentSpeed - Model.MaxSpeed / 20.0f);
}

std::vector<ISFCPowerConsumer*> UShipMovementComponent::GetPowerConsumers() {
    return { this };
}

float UShipMovementComponent::ConsumePower(uint8 Priority, float AvailablePower, float TurnFraction) {
    // Only draw power at PRIORITY_MOVEMENT.
    if (Priority != PRIORITY_MOVEMENT) return 0.0f;

    float ConsumedPower = CurrentSpeed * Model.MovementPowerCost * TurnFraction;
    if (ConsumedPower > AvailablePower) {
        ConsumedPower = AvailablePower;
        CurrentSpeed = ConsumedPower / Model.MovementPowerCost / TurnFraction;
    }
    return ConsumedPower;
}

// Called every frame
void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Move plan forwards (with sweep so we stop when we collide with things)
    GetOwner()->AddActorLocalOffset(
        FVector(CurrentSpeed * DeltaTime * SPEED_TO_ENGINE_MOVEMENT, 0.f, 0.f), true);
}

