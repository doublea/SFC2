// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShipPawn.h"
#include "SFCPlayerController.generated.h"

class UTargetManager;

/**
*
*/
UCLASS()
class SFC2_API ASFCPlayerController : public APlayerController
{
	GENERATED_BODY()

	ASFCPlayerController();

	void SetupInputComponent();

public:
	FVector CurrentDestination;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UUserWidget* HUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTargetManager* TargetManager;
    UTargetManager* GetTargetManager() const { return TargetManager; }

	UFUNCTION(BlueprintPure, Category = "Hud")
	float GetObjectScreenRadius(AActor* InActor);

	void Possess(APawn* InPawn) override;

protected:
	virtual void PlayerTick(float DeltaTime) override;

private:
	TSubclassOf<UUserWidget> HUDClass;
	void LeftClickPressed();
	void RightClickPressed();
	void SpeedUp();
	void SpeedDown();
    void FireSelected();
    void FireAll();

	AShipPawn* GetCurrentShip() const;
};
