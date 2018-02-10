// Fill out your copyright notice in the Description page of Project Settings.

#include "SFCPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Object.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Camera/SFCPlayerCameraManager.h"
#include "ShipPawn.h"
#include "Targeting/TargetManager.h"
#include "Components/PrimitiveComponent.h"
#include "Runtime/UMG/Public/Components/GridPanel.h"
#include "SFCUtils.h"

ASFCPlayerController::ASFCPlayerController() {
	// Configure engine options
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableMouseOverEvents = true;
    bAutoManageActiveCameraTarget = false;

    PlayerCameraManagerClass = ASFCPlayerCameraManager::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> BlueprintClass(TEXT("/Game/HUD/ShipHUD"));
	if (BlueprintClass.Succeeded()) {
		HUDClass = BlueprintClass.Class;
	}

    TargetManager = NewObject<UTargetManager>();
    TargetManager->OnTargetAcquired().AddLambda([this] (AActor* Target) {
        ASFCPlayerCameraManager* CameraManager = Cast<ASFCPlayerCameraManager>(PlayerCameraManager);
        CameraManager->SetTargetActor(Target);
    });
}

void ASFCPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ASFCPlayerController::LeftClickPressed);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &ASFCPlayerController::RightClickPressed);
	InputComponent->BindAction("SpeedUp", IE_Repeat, this, &ASFCPlayerController::SpeedUp);
	InputComponent->BindAction("SpeedUp", IE_Pressed, this, &ASFCPlayerController::SpeedUp);
	InputComponent->BindAction("SpeedDown", IE_Repeat, this, &ASFCPlayerController::SpeedDown);
	InputComponent->BindAction("SpeedDown", IE_Pressed, this, &ASFCPlayerController::SpeedDown);
	InputComponent->BindAction("FireSelected", IE_Pressed, this, &ASFCPlayerController::FireSelected);
	InputComponent->BindAction("FireAll", IE_Pressed, this, &ASFCPlayerController::FireAll);
}

void ASFCPlayerController::Possess(APawn* InPawn) {
	Super::Possess(InPawn);

	if (HUDClass == nullptr) {
		return;
	}
    if (HUDWidget != nullptr) {
        HUDWidget->RemoveFromViewport();
        HUDWidget = nullptr;
    }
    HUDWidget = CreateWidget<UUserWidget>(this, HUDClass);
    CHECK(HUDWidget != nullptr);
    HUDWidget->AddToViewport();

    ASFCPlayerCameraManager* CameraManager = Cast<ASFCPlayerCameraManager>(PlayerCameraManager);
    CameraManager->SetAttachedActor(InPawn);
}

AShipPawn* ASFCPlayerController::GetCurrentShip() const {
	AShipPawn* pawn = Cast<AShipPawn>(GetPawn());
	CHECKMSG(pawn, "ASFCPlayerController not handling AShipPawn");
	return pawn;
}

void ASFCPlayerController::PlayerTick(float DeltaTime) {
    // TODO: Consider disabling ticks if we don't need them.
	Super::PlayerTick(DeltaTime);
}

// ------------------------------------------
// Player action handlers (turn, fire, etc.)
// ------------------------------------------

void ASFCPlayerController::LeftClickPressed() {
	APawn* const MyPawn = GetPawn();
	if (MyPawn) {
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_WorldStatic, false, Hit);  // ECC_WorldStatic hits the invisible plane

		if (Hit.bBlockingHit) {
			FVector src = MyPawn->GetActorLocation();
			FVector dst = Hit.ImpactPoint;
			FVector direction(FVector(dst.X, dst.Y, 0) - FVector(src.X, src.Y, 0));
			direction.Normalize();
			SetControlRotation(direction.Rotation());
		}
	}
}

void ASFCPlayerController::RightClickPressed() {
	FHitResult Hit;
	// TODO: Maybe use a special "targetable" channel for this? Make
	// TargetComponent set up special collison for this channel on its root
	// component?
	GetHitResultUnderCursor(ECC_WorldDynamic, false, Hit);  // ECC_WorldDynamic hits things that move around (like ships).

	if (Hit.bBlockingHit) {
		AActor* actor = Hit.Actor.Get();
		if (actor != nullptr) {
            TargetManager->NewTargetSelected(actor);
		}
	}
}

void ASFCPlayerController::SpeedUp() {
	GetCurrentShip()->SpeedUp();
}

void ASFCPlayerController::SpeedDown() {
	GetCurrentShip()->SpeedDown();
}

void ASFCPlayerController::FireAll() {
    AActor* target = TargetManager->GetCurrentTarget();
    if (target == nullptr) {
        DEBUGMSG("No target selected!");
        return;
    }
    DEBUGMSG("Firing!");
    AShipPawn* ship = GetCurrentShip();
    ship->FireAll(target);
}

void ASFCPlayerController::FireSelected() {
    // TODO
    FireAll();
}


// ----------------------------------------------------------------
// Cruft below left here to do reticle scaling later if necessary.
// ----------------------------------------------------------------

#if WITH_EDITOR
/* Get Screen Percentage */
static const auto CVarScreenPercentage = IConsoleManager::Get().FindTConsoleVariableDataFloat(TEXT("r.SCreenPercentage"));
#endif  // WITH_EDITOR

float ASFCPlayerController::GetObjectScreenRadius(AActor* InActor)
{
	if (!InActor) {
		return 0.f;
	}
	float ScreenRadius;
	int32 Width, Height;
	FVector Viewlocation;
	FRotator ViewRotation; // Not Used, but required for Function call
	float CamFOV = PlayerCameraManager->GetFOVAngle();
#if WITH_EDITOR
	float ScreenPerc = CVarScreenPercentage->GetValueOnGameThread() / 100.0f;
#endif  // WITH_EDITOR

	/* Get the size of the viewport, and the player cameras location. */
	GetViewportSize(Width, Height);
	GetPlayerViewPoint(Viewlocation, ViewRotation);

#if WITH_EDITOR
	/* Factor in Screen Percentage & Quality Settings */
	Width *= ScreenPerc;
	Height *= ScreenPerc;
#endif  // WITH_EDITOR

	/* Easy Way To Return The Size, Create a vector and scale it. Alternative would be to use FMath::Max3 */
	float SRad = FVector2D(Width, Height).Size();

	/* Get Object Bounds (R) */
	if (!InActor->GetRootComponent()) {
		return 0.f;
	}
	float BoundingRadius = InActor->GetRootComponent()->Bounds.SphereRadius;
	float DistanceToObject = FVector(InActor->GetActorLocation() - Viewlocation).Size();

	/* Get Projected Screen Radius */
	ScreenRadius = FMath::Atan(BoundingRadius / DistanceToObject);
	ScreenRadius *= SRad / FMath::DegreesToRadians(CamFOV);

	return ScreenRadius;
}