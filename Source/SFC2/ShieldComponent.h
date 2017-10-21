// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShieldComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UShieldComponent : public USceneComponent
{
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UParticleSystemComponent* ShieldParticleSystem;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ShieldMesh;

public:	
	// Sets default values for this component's properties
	UShieldComponent();

    UFUNCTION()
    void ShieldCollision(const FHitResult& HitInfo);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
