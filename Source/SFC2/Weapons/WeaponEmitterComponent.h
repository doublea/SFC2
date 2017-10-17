// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponEmitterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UWeaponEmitterComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponEmitterComponent();

    UPROPERTY()
    class UParticleSystemComponent* PhaserParticleSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* Mesh;

    void FireAtTarget(AActor* target);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
