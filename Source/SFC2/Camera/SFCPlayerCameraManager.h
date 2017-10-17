// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SFCPlayerCameraManager.generated.h"

UENUM(BlueprintType)
enum class ECameraMode : uint8 {
        CM_Free,
        CM_TrackTarget,
};

/**
 * 
 */
UCLASS()
class SFC2_API ASFCPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	
public:
    ASFCPlayerCameraManager();
    virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	
    UFUNCTION()
    void SetAttachedActor(AActor* Actor);

    UFUNCTION()
    void SetTargetActor(AActor* Target) { TargetActor = Target;  }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECameraMode CameraMode;

protected:
    UPROPERTY()
    class USpringArmComponent* SpringArm;

    UPROPERTY()
    class UCameraComponent* Camera;

    UPROPERTY()
    class AActor* AttachedActor;

    UPROPERTY()
    class AActor* TargetActor;
};
