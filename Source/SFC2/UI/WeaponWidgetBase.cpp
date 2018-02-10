// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponWidgetBase.h"
#include "Models/ShipModel.h"
#include "SFCUtils.h"

void UWeaponWidgetBase::NativeConstruct() {
    Super::NativeConstruct();

    UTexture2D* PhaserTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/HUD/Images/Phaser"), nullptr, LOAD_None, nullptr);
    CHECK(PhaserTexture != nullptr);
    WeaponIcons[EWeaponType::WT_Phaser] = PhaserTexture;

    UTexture2D* PhotonTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/HUD/Images/Photon"), nullptr, LOAD_None, nullptr);
    CHECK(PhotonTexture != nullptr);
    WeaponIcons[EWeaponType::WT_Photon] = PhotonTexture;
}

void UWeaponWidgetBase::Init(const FWeaponModel& WeaponModel) {
    this->WeaponModel = &WeaponModel;
}

float UWeaponWidgetBase::GetChargedFraction() const {
    return WeaponModel->Charge.GetChargeFraction();
}

EWeaponType UWeaponWidgetBase::GetWeaponType() const {
    if (WeaponModel == nullptr) return EWeaponType::WT_Unknown;
    return WeaponModel->Type;
}

UTexture2D* UWeaponWidgetBase::GetWeaponIcon() const {
    auto it = WeaponIcons.find(GetWeaponType());
    if (it != WeaponIcons.end()) {
        return it->second;
    }
    return nullptr;
}