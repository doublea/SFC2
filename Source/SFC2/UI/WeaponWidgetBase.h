// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Models/ShipModel.h"
#include <unordered_map>
#include "WeaponWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SFC2_API UWeaponWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    float GetChargedFraction() const;

    UFUNCTION(BlueprintCallable)
    EWeaponType GetWeaponType() const;

    UFUNCTION(BlueprintCallable)
    UTexture2D* GetWeaponIcon() const;

    UFUNCTION(BlueprintCallable)
    void Init(const FWeaponModel& Weapon);

private:

    const FWeaponModel* WeaponModel;
    std::unordered_map<EWeaponType, UTexture2D*> WeaponIcons;
	
};
