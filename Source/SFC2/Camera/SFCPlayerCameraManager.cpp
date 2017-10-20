// Fill out your copyright notice in the Description page of Project Settings.

#include "SFCPlayerCameraManager.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"

static const float kSFCCameraTilt = -20.0f;

ASFCPlayerCameraManager::ASFCPlayerCameraManager() {
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->TargetArmLength = 480.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0., 0.f, 60.f);
	SpringArm->SetRelativeRotation(FRotator(kSFCCameraTilt, 0.f, 0.f));  // Tilt the camera downwards.
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 15.f;
    // SpringArm ignores collisions with its Owner by default. This works when
    // it's attached to its owner, but in this case the owner is the
    // CameraManager. If we don't disable this, it will collide with the
    // ShipPawn and stick the camera inside the ShipPawn mesh.
    // Here's another way to do it:
    // https://forums.unrealengine.com/development-discussion/c-gameplay-programming/83231-player-camera-in-controller-instead-of-character
    SpringArm->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller
	Camera->SetActive(true);

    CameraMode = ECameraMode::CM_TrackTarget;
}

void ASFCPlayerCameraManager::SetAttachedActor(AActor* Actor) {
    SpringArm->AttachToComponent(
        Actor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ASFCPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) {
    if (CameraMode == ECameraMode::CM_TrackTarget && TargetActor != nullptr) {
        // We want to keep the AttachedActor in view, so we need to rotate the
        // SpringArm so that SpringArm -> AttachedActor and Target are all
        // aligned.
        FRotator ToTarget = (TargetActor->GetActorLocation() - Camera->GetComponentLocation()).ToOrientationRotator();
        ToTarget.Normalize();
        FRotator SpringArmRotation;
        SpringArmRotation.Pitch = kSFCCameraTilt;
        SpringArmRotation.Yaw = ToTarget.Yaw;
        SpringArmRotation.Roll = 0.0f;
        SpringArmRotation.Normalize();
        SpringArm->SetWorldRotation(SpringArmRotation);
        Camera->SetWorldRotation(ToTarget);
    }
    Camera->GetCameraView(DeltaTime, OutVT.POV);
}

