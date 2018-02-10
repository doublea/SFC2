// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/DamageType.h"
#include <cmath>
#include "DamageTypes.generated.h"

UCLASS(const, Blueprintable, BlueprintType)
class SFC2_API USFCDamageType : public UDamageType {
    GENERATED_BODY()

public:
    USFCDamageType();

    UFUNCTION(BlueprintCallable)
    virtual float GetDamageAtDistance(float BaseDamage, float Distance) const { return BaseDamage / Distance; }
};
/**
 * 
 */
UCLASS(const, Blueprintable, BlueprintType)
class SFC2_API USFCNormalPhotonDamageType : public USFCDamageType
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    virtual float GetDamageAtDistance(float BaseDamage, float Distance) const override { return 8; }
};

UCLASS(const, Blueprintable, BlueprintType)
class SFC2_API USFCOverloadPhotonDamageType : public USFCDamageType
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    virtual float GetDamageAtDistance(float BaseDamage, float Distance) const override { return 16; }
};

UCLASS(const, Blueprintable, BlueprintType)
class SFC2_API USFCProximityPhotonDamageType : public USFCDamageType
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    virtual float GetDamageAtDistance(float BaseDamage, float Distance) const override { return 4; }
};


UCLASS(const, Blueprintable, BlueprintType)
class SFC2_API USFCPhaserDamageType : public USFCDamageType
{
    GENERATED_BODY()
};
