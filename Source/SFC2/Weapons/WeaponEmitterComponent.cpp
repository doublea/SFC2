// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponEmitterComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ShipPawn.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UWeaponEmitterComponent::UWeaponEmitterComponent()
{
    bAutoActivate = true;
    bAutoRegister = true;
}


// Called when the game starts
void UWeaponEmitterComponent::BeginPlay()
{
	Super::BeginPlay();
}