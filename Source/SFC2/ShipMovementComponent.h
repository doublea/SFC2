// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Models/ShipModel.h"
#include "SFCPoweredSystem.h"
#include <vector>
#include "ShipMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UShipMovementComponent : public UActorComponent, public ISFCPoweredSystem, public ISFCPowerConsumer
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipMovementComponent();

    UFUNCTION(BlueprintCallable)
    void Init(FMovementModel MovementModel);

    UPROPERTY(BlueprintReadOnly)
    float CurrentSpeed;

	void SetSpeedFraction(float speedFraction);
	void SpeedUp();
	void SpeedDown();

    virtual std::vector<ISFCPowerConsumer*> GetPowerConsumers() override;
    virtual float ConsumePower(uint8 Priority, float AvailablePower, float TurnFraction) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    FMovementModel Model;
};
