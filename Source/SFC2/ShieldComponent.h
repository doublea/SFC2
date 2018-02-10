// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SFCPoweredSystem.h"
#include "ShieldComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UShieldComponent : public USceneComponent, public ISFCPoweredSystem, public ISFCPowerConsumer
{
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UParticleSystemComponent* ShieldParticleSystem;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ShieldMesh;

public:	
	// Sets default values for this component's properties
	UShieldComponent();

    UFUNCTION()
    void ShieldCollision(const FVector& HitDirection);

    // From ISFCPoweredSystem
    virtual std::vector<ISFCPowerConsumer*> GetPowerConsumers() override;
    // From ISFCPowerConsumer
    virtual float ConsumePower(uint8 Priority, float AvailablePower, float TurnFraction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
