// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Targeting/TargetManager.h"
#include "HUD/ReticleWidget.h"
#include "SFCPlayerController.h"
#include "SFCUtils.h"


void ActivateComponent(USceneComponent* Component) {
    Component->SetVisibility(true);
    Component->SetHiddenInGame(false);
}

void DeactivateComponent(USceneComponent* Component) {
    Component->SetVisibility(false);
    Component->SetHiddenInGame(true);
}

// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(false);

	struct FConstructorStatics
	{
		ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass;
		FConstructorStatics()
			: WidgetClass(TEXT("/Game/HUD/TargetReticle")) {}
	};
	static FConstructorStatics ConstructorStatics;

	CHECKMSG(ConstructorStatics.WidgetClass.Succeeded(),
			 "Could not find reticle widget class.");
	ReticleWidgetClass = ConstructorStatics.WidgetClass.Class;
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetReticle"));
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	isTarget = false;
}

// Called when the game starts
void UTargetComponent::BeginPlay() {
	Super::BeginPlay();

	if (ReticleWidget == nullptr && ReticleWidgetClass != nullptr) {
		ReticleWidget = CreateWidget<UReticleWidget>(
			GetWorld()->GetGameInstance(), ReticleWidgetClass);
        ReticleWidget->TargetActor = GetOwner();
		WidgetComponent->SetWidget(ReticleWidget);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
        DeactivateComponent(WidgetComponent);
		DEBUGMSG("Created widget component");
	}

	if (WidgetComponent != nullptr) {
		WidgetComponent->SetWorldLocation(GetOwner()->GetActorLocation());
		WidgetComponent->AttachToComponent(
			GetOwner()->GetRootComponent(),
			FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

    // If MouseOverComponent is not set, try to detect it.
    if (MouseOverComponent == nullptr) {
        MouseOverComponent = Cast<UPrimitiveComponent>(
            GetOwner()->GetRootComponent());
        CHECKMSG(
            MouseOverComponent != nullptr,
            "TargetComponent: MouseOverComponent not set, and actor does not have a UPrimitiveComponent root component.");
    }

	DEBUGMSG_FSTRING(GetOwner()->GetName());
	FScriptDelegate beginCursorOverDelegate;
	beginCursorOverDelegate.BindUFunction(this, "OnBeginCursorOver");
	MouseOverComponent->OnBeginCursorOver.Add(beginCursorOverDelegate);

	FScriptDelegate endCursorOverDelegate;
	endCursorOverDelegate.BindUFunction(this, "OnEndCursorOver");
	MouseOverComponent->OnEndCursorOver.Add(endCursorOverDelegate);

	// Subscribe to click events
	ASFCPlayerController* controller = Cast<ASFCPlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CHECK(controller);
    UTargetManager* targetManager = controller->GetTargetManager();
	targetManager->OnTargetSelected().AddLambda([this, targetManager] (AActor* target) {
		if (target == GetOwner()) {
            if (!targetManager->NewTargetAcquired(GetOwner())) return;
			const static FString msg = FString(TEXT("New target is "));
			DEBUGMSG_FSTRING(msg + GetOwner()->GetName());
			isTarget = true;
            ActivateComponent(WidgetComponent);
		} else if (isTarget) {
			const static FString msg = FString(TEXT("Old target was "));
			DEBUGMSG_FSTRING(msg + GetOwner()->GetName());
			isTarget = false;
            DeactivateComponent(WidgetComponent);
		}
	});

}

void UTargetComponent::OnBeginCursorOver(UPrimitiveComponent* touchedComponent) {
	if (isTarget) return;
	static const FString msg = FString(TEXT("OnBeginCursorOver for "));
	DEBUGMSG_FSTRING(msg + GetOwner()->GetName());
    ActivateComponent(WidgetComponent);
}

void UTargetComponent::OnEndCursorOver(UPrimitiveComponent* touchedComponent) {
	if (isTarget) return;
	static const FString msg = FString(TEXT("OnEndCursorOver for "));
	DEBUGMSG_FSTRING(msg + GetOwner()->GetName());
    DeactivateComponent(WidgetComponent);
}

// Called every frame
void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// TODO: Scale reticle.
}

