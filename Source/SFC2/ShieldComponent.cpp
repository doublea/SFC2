// Fill out your copyright notice in the Description page of Project Settings.

#include "ShieldComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetRenderingLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "SFCUtils.h"


// Sets default values for this component's properties
UShieldComponent::UShieldComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ShieldMesh;
        ConstructorHelpers::FObjectFinder<UMaterial> ShieldMaterial;
        ConstructorHelpers::FObjectFinder<UMaterial> ImpactPointMaterial;
		FConstructorStatics()
            : ShieldMesh(TEXT("/Game/Meshes/Shape_Sphere")),
              ShieldMaterial(TEXT("/Game/Materials/M_Shield2")),
              ImpactPointMaterial(TEXT("/Game/Materials/M_ImpactPoint")) {}
	};
	static FConstructorStatics ConstructorStatics;

    ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
    if (ConstructorStatics.ShieldMesh.Succeeded()) {
        ShieldMesh->SetStaticMesh(ConstructorStatics.ShieldMesh.Get());
    }
    ShieldMesh->SetVisibility(false);
    ShieldMaterial = ConstructorStatics.ShieldMaterial.Object;
    ImpactPointMaterial = ConstructorStatics.ImpactPointMaterial.Object;
}


// Called when the game starts
void UShieldComponent::BeginPlay()
{
	Super::BeginPlay();

    ShieldMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
    ShieldMesh->SetWorldLocation(GetOwner()->GetActorLocation());

    ShieldMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    ShieldMesh->SetCollisionResponseToChannel(ECC_SFCWeaponTraceChannel, ECollisionResponse::ECR_Block);
    ShieldMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ShieldMesh->bGenerateOverlapEvents = false;

    RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetOwner(), 1024, 1024);
    UKismetRenderingLibrary::ClearRenderTarget2D(GetOwner(), RenderTarget);
    ShieldMaterialInstance = UMaterialInstanceDynamic::Create(ShieldMaterial, this);
    ShieldMaterialInstance->SetTextureParameterValue(TEXT("ImpactTexture"), RenderTarget);
    // This line sometimes crashes? WTF?
    ShieldMesh->SetMaterial(0, ShieldMaterialInstance);
    ImpactPointMaterialInstance = UMaterialInstanceDynamic::Create(ImpactPointMaterial, this);
}

void UShieldComponent::ShieldCollision(const FHitResult& HitInfo) {
    FVector2D CollisionUV;
    if (HitInfo.GetComponent() != nullptr) {
        DEBUGMSG("Hit component!");
        DEBUGMSG_FSTRING(HitInfo.GetComponent()->GetFullName());
        DEBUGMSG_FSTRING(FString::FromInt(HitInfo.FaceIndex));
    }
    if (UGameplayStatics::FindCollisionUV(HitInfo, 0, CollisionUV)) {
        DEBUGMSG_FSTRING(TEXT("Found Collision UV! ") + CollisionUV.ToString());
    }
    else {
        DEBUGMSG("Failed to find Collision UV.");
    }
    ImpactPointMaterialInstance->SetVectorParameterValue(
        TEXT("ForcePosition"), UKismetMathLibrary::Conv_Vector2DToVector(CollisionUV));
    //UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetOwner(), RenderTarget, ImpactPointMaterialInstance);
    ImpactStartedAt = GetWorld()->GetUnpausedTimeSeconds();
    ShieldMesh->SetVisibility(true);
}

// Called every frame
void UShieldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    float AnimTime = GetWorld()->GetUnpausedTimeSeconds() - ImpactStartedAt;
    if (AnimTime <= 0.25f) {
        ImpactPointMaterialInstance->SetScalarParameterValue(TEXT("Time"), AnimTime);
        UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetOwner(), RenderTarget, ImpactPointMaterialInstance);
    } else if (ImpactStartedAt != 0.0f) {
        UKismetRenderingLibrary::ClearRenderTarget2D(GetOwner(), RenderTarget);
        ImpactStartedAt = 0.0f;
        ShieldMesh->SetVisibility(false);
    }
	// ...
}

