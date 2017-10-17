// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipPawn.h"
#include "ReticleWidget.generated.h"


/**
 * 
 */
UCLASS()
class SFC2_API UReticleWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class AActor* TargetActor;

    UFUNCTION(BlueprintCallable)
    void TargetAsShipPawn(AShipPawn*& OutShipPawn, bool& Success) const {
        AShipPawn* ship = Cast<AShipPawn>(TargetActor);
        if (ship != nullptr) {
            OutShipPawn = ship;
            Success = true;
        }
        else {
            OutShipPawn = nullptr;
            Success = false;
        }
    }
};
