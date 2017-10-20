// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Models/ShipModel.h"
#include "ShipPawn.generated.h"

UCLASS(Config = Game)
class AShipPawn : public APawn
{
    GENERATED_BODY()

    // A dummy component to be used as root. This is so we can swap the
    // ShipMesh component underneath in runtime when we parse the desired
    // mesh from ShipModel.
    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USceneComponent* TheRootComponent;

    /** StaticMesh component that will be the visuals for our flying pawn */
    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ShipMesh;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UTurnIndicatorComponent* TurnIndicator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UTargetComponent* TargetComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UWeaponsManagerComponent* WeaponsManagerComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UParticleSystemComponent* ShieldParticleSystem;

    // Shield magic
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UShieldComponent* ShieldComponent;

public:
	AShipPawn();
	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;

	// End AActor overrides
	void SetSpeedFraction(float speedFraction);
	void SpeedUp();
	void SpeedDown();
    void FireAll(AActor* Target);
    // TODO: void FireWeapon(int WeaponIdx, AActor* target);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FShipModel ShipModel;

	float GetCurrentTurnRate();

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return ShipModel.Hull.Damage.IsDestroyed(); }

    UFUNCTION()
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMesh() const { return ShipMesh; }

private:
};
