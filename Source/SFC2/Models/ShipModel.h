// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipModel.generated.h"

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
    int Charge;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int MaxCharge;
};

USTRUCT(BlueprintType)
struct FMovementModel {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentSpeed;

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
    int CurrentPower;

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