// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnIndicatorComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ShipPawn.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UTurnIndicatorComponent::UTurnIndicatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> TurnIndicatorMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Meshes/Shape_Plane.Shape_Plane")),
			TurnIndicatorMaterial(TEXT("Material'/Game/Materials/TurnIndicatorMaterial.TurnIndicatorMaterial'")) {}
	};
	static FConstructorStatics ConstructorStatics;

	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurnIndicatorPlane0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	PlaneMesh->SetRelativeScale3D(FVector(5.0f, 5.0f, 0.0f));
	PlaneMesh->bGenerateOverlapEvents = false;
	PlaneMesh->SetEnableGravity(false);
	Material = ConstructorStatics.TurnIndicatorMaterial.Get();
	PlaneMesh->SetMaterial(0, Material);
}


// Called when the game starts
void UTurnIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	PlaneMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PlaneMesh->SetWorldLocation(GetOwner()->GetActorLocation());
	MaterialInstance = UMaterialInstanceDynamic::Create(PlaneMesh->GetMaterial(0), this);
	if (MaterialInstance == nullptr) {
		DEBUGMSG("Whhaaaaaa");
	}
	PlaneMesh->SetMaterial(0, MaterialInstance);
}


// Called every frame
void UTurnIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AShipPawn* owner = Cast<AShipPawn>(GetOwner());

	FRotator rot = owner->GetActorRotation() - owner->GetControlRotation();
	rot.Normalize();
	if (!rot.IsNearlyZero(1.0f)) {
		float direction = rot.Yaw < 0.0f ? 1.0f : -1.0f;
		float turnAmount = rot.Yaw / 360.0f;
		MaterialInstance->SetScalarParameterValue("TurnAmount", turnAmount);
	}
	else {
		MaterialInstance->SetScalarParameterValue("TurnAmount", 0.0f);
	}
}



