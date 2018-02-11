// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "UObject/NoExportTypes.h"
#include "WeaponEmitterComponent.h"
#include "PhotonEmitterComponent.generated.h"

/**
 * 
 */
UCLASS()
class SFC2_API UPhotonEmitterComponent : public UWeaponEmitterComponent
{
    GENERATED_BODY()

    UPROPERTY()
    TSubclassOf<AWeaponActor> PhotonBlueprint;

    UPROPERTY()
    class USoundBase* PhotonSound;
	
public:
    UPhotonEmitterComponent();

    UFUNCTION(BlueprintCallable)
    virtual bool FireAtTarget(const FWeaponModel& WeaponState, AActor* Target) override;
	
};
