// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponEmitterComponent.h"
#include "PhaserEmitterComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UPhaserEmitterComponent : public UWeaponEmitterComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhaserEmitterComponent();

    UPROPERTY()
    class UParticleSystemComponent* PhaserParticleSystem;

    UFUNCTION(BlueprintCallable)
    virtual bool FireAtTarget(AActor* Target) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
