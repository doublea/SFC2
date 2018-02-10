// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <algorithm>
#include "ShipModel.generated.h"

USTRUCT(BlueprintType)
struct FSystemCharge {
    GENERATED_BODY()

public:
    // Current accumulated charge.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentCharge = 0.0f;

    // Max accumulated charge.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxCharge = 1.0f;

    // System power draw per turn when fully charged.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChargedPowerDraw = 1.0f;

    // System power draw per turn when charging.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChargingPowerDraw = 1.0f;

    // If available power is less than power draw, how much to discharge per turn.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DischargeRate = 0.5f;

    bool IsCharged() const { return CurrentCharge >= MaxCharge; }

    float GetChargeFraction() const { return CurrentCharge / MaxCharge; }

    void Discharge() { CurrentCharge = 0.0f; }

    /** Attempts to charge the system given some amount of power. 
     *  The returned value is the amount of power consumed.
     **/
    float AddCharge(float AvailablePower, float TurnFraction) {
        float RequiredPower = (IsCharged() ? ChargedPowerDraw : ChargingPowerDraw) * TurnFraction;
        if (AvailablePower >= RequiredPower) {
            CurrentCharge = std::min(CurrentCharge + RequiredPower, MaxCharge);
            return RequiredPower;
        }
        CurrentCharge = std::max(0.0f, CurrentCharge - DischargeRate * TurnFraction);
        return 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSystemDamage {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int CurrentDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int MaxDamage;

    bool IsDestroyed() const { return CurrentDamage >= MaxDamage; }
};

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
    WT_Unknown,
    WT_Phaser,
    WT_Photon,
};

UENUM(BlueprintType)
enum class EWeaponMode : uint8 {
    WM_Normal,
    WM_Photon_Normal,
    WM_Photon_Overload,
    WM_Photon_Proximity,
};

USTRUCT(BlueprintType)
struct FWeaponModel {
    GENERATED_BODY()

    // Mesh socket where this weapon is located in the ship mesh.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWeaponType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWeaponMode WeaponMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSystemDamage Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSystemCharge Charge;
};

USTRUCT(BlueprintType)
struct FMovementModel {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int TurnClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementPowerCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Acceleration;
};

USTRUCT(BlueprintType)
struct FPowerSystemModel {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int MaxPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSystemDamage Damage;
};

USTRUCT(BlueprintType)
struct FHullModel {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int MaxHullPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int CurrentHullPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSystemDamage Damage;
};

/**
 * FShipModel describes the state of a ship.
 * It will be used by AShipPawn and its components to implement the appropriate
 * behavior (e.g. WeaponsManagerComponent will read this to start tracking the
 * state of available weapons).
 */
USTRUCT(BlueprintType)
struct FShipModel {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ShipClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMovementModel Movement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FWeaponModel> Weapons;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPowerSystemModel PowerSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHullModel Hull;

    bool IsDead() const { return Hull.Damage.IsDestroyed(); }
};

/**
 * FShipList contains all ships in the game. It's used to load available ships
 * at the start of the game.
 */
USTRUCT(BlueprintType)
struct FShipList {
    GENERATED_BODY()

    TArray<FShipModel> Ships;

    static FShipList* LoadFromShiplistFile(char* path);
};