// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Models/ShipModel.h"
#include "WeaponEmitterComponent.generated.h"


/**
 * UWeaponEmitterComponent is the base class for all components implementing weapon types.
 */
UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UWeaponEmitterComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponEmitterComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SocketName;

    UFUNCTION(BlueprintCallable)
    virtual bool FireAtTarget(AActor* Target) PURE_VIRTUAL(UWeaponEmitterComponent::FireAtTarget, return false;);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
