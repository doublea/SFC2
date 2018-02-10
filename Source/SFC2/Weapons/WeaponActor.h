// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Models/DamageTypes.h"
#include "WeaponActor.generated.h"

/** Base class for all weapon types that require an actor (e.g. missiles, plasma, photons). **/
UCLASS(Blueprintable, BlueprintType)
class SFC2_API AWeaponActor : public AActor
{
    GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AWeaponActor();

    // The actor that fired this.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class AActor* SourceActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class AActor* TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<USFCDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UBoxComponent* Collision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UProjectileMovementComponent* ProjectileMovement;

    void Init(AActor* Source, AActor* Target, TSubclassOf<USFCDamageType> DamageType);
};
