// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFCPoweredSystem.h"
#include "Models/ShipModel.h"
#include <vector>
#include "ShipPowerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UShipPowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipPowerComponent();

    void Init(const FPowerSystemModel PowerSystemModel, std::vector<ISFCPoweredSystem*> System);

    UPROPERTY(BlueprintReadOnly)
    float CurrentPower;

    UPROPERTY(BlueprintReadOnly)
    float CurrentExcessPower;

    UPROPERTY(BlueprintReadOnly)
    FPowerSystemModel PowerSystemModel;

    // To be called every tick to update power state for all configured systems.
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
    // Systems that need to be fed power.
    std::vector<ISFCPoweredSystem*> Systems;
};
