// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ShipPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "TurnIndicatorComponent.h"
#include "Targeting/TargetComponent.h"
#include "Weapons/WeaponsManagerComponent.h"
#include "Weapons/PhaserEmitterComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Models/DamageTypes.h"
#include "SFCUtils.h"
#include "ShieldComponent.h"
#include <algorithm>

AShipPawn::AShipPawn()
{
	bAllowTickBeforeBeginPlay = false;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ShipMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInterface> UFOMaterial;
        FConstructorStatics()
            : ShipMesh(TEXT("/Game/Meshes/UFO.UFO")),
              UFOMaterial(TEXT("/Game/Materials/UFOMaterial")) {}
	};
	static FConstructorStatics ConstructorStatics;

	TheRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = TheRootComponent;

	// Create static mesh component
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh0"));
    ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetStaticMesh(ConstructorStatics.ShipMesh.Get());	// Set static mesh
	ShipMesh->SetMaterial(0, ConstructorStatics.UFOMaterial.Get());
	ShipMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Don't rotate on controller rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	TurnIndicator = CreateDefaultSubobject<UTurnIndicatorComponent>(TEXT("TurnIndicator0"));
	TurnIndicator->SetupAttachment(RootComponent);

	TargetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("Target"));
    TargetComponent->MouseOverComponent = ShipMesh;

    WeaponsManagerComponent = CreateDefaultSubobject<UWeaponsManagerComponent>(TEXT("WeaponsEmitter"));
    WeaponsManagerComponent->Mesh = ShipMesh;
    WeaponsManagerComponent->WeaponModels = &ShipModel.Weapons;

    ShieldComponent = CreateDefaultSubobject<UShieldComponent>(TEXT("Shield"));
    ShieldComponent->SetupAttachment(RootComponent);
}

void AShipPawn::BeginPlay() {
    Super::BeginPlay();
    // Initialize all properties from the ShipModel
    // TODO: A lot of stuff

    if (ShipModel.Mesh != NAME_None) {
        // FIXME: Looks like loading meshes like this returns null?
        UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *ShipModel.Mesh.ToString()));
        CHECK(mesh);
        ShipMesh->SetStaticMesh(mesh);
        ShipMesh->SetWorldLocation(GetActorLocation());
        ShipMesh->SetWorldRotation(GetActorRotation());
        ShipMesh->SetVisibility(true);
    }
}

float AShipPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    // This crashes :(
    // const USFCDamageType* SFCDamageType = Cast<USFCDamageType>(DamageType);
    UObject* DamageTypeUObj = DamageEvent.DamageTypeClass->GetDefaultObject();
    DEBUGMSG_FSTRING(DamageEvent.DamageTypeClass->GetName());
    USFCDamageType* DamageType = Cast<USFCDamageType>(DamageTypeUObj);
    if (DamageType == nullptr) {
        return 0.0f;
    }
    float ActualDamage = DamageType->GetDamageAtDistance(DamageAmount, FVector::Dist(GetActorLocation(), DamageCauser->GetActorLocation()));
    FString msg("Took ");
    FString dmg_str = msg.SanitizeFloat(ActualDamage);
    msg.Append(dmg_str + " damage!");
    DEBUGMSG_FSTRING(msg);

    // Shield Magic!
    FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
    ShieldComponent->ShieldCollision(PointDamageEvent->HitInfo);
    return Super::TakeDamage(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
}

float AShipPawn::GetCurrentTurnRate()
{
	return (20.0f / ShipModel.Movement.TurnClass) * (500 / (ShipModel.Movement.CurrentSpeed + 400));
}

void AShipPawn::SetSpeedFraction(float speedFraction) {
	CHECK(0.0f <= speedFraction && speedFraction <= 1.0f);
	ShipModel.Movement.CurrentSpeed = ShipModel.Movement.MaxSpeed * speedFraction;
}

void AShipPawn::SpeedUp() {
	ShipModel.Movement.CurrentSpeed = std::min(
        ShipModel.Movement.MaxSpeed,
        ShipModel.Movement.CurrentSpeed + ShipModel.Movement.MaxSpeed / 20.0f);
}

void AShipPawn::SpeedDown() {
	ShipModel.Movement.CurrentSpeed = std::max(
        0.0f, ShipModel.Movement.CurrentSpeed - ShipModel.Movement.MaxSpeed / 20.0f);
}

void AShipPawn::FireAll(AActor* target) {
    for (int i = 0; i < ShipModel.Weapons.Num(); i++) {
        WeaponsManagerComponent->FireWeapon(i, target);
    }
}

FVector SpeedToLocalMove(float CurrentSpeed, float DeltaSeconds) {
    return FVector(CurrentSpeed * DeltaSeconds, 0.f, 0.f);
}

void AShipPawn::Tick(float DeltaSeconds)
{
	const FVector LocalMove = FVector(
        ShipModel.Movement.CurrentSpeed * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(
        SpeedToLocalMove(ShipModel.Movement.CurrentSpeed, DeltaSeconds), true);

	FRotator rot = GetActorRotation() - GetControlRotation();
	rot.Normalize();
	if (!rot.IsNearlyZero(1.0f)) {
		float direction = rot.Yaw < 0.0f ? 1.0f : -1.0f;

		// Calculate change in rotation this frame
		FRotator DeltaRotation(0, direction * GetCurrentTurnRate() * DeltaSeconds, 0);

		// Rotate plane
		AddActorWorldRotation(DeltaRotation);
	}

	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}
