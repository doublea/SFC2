// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTargetComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		TSubclassOf<class UUserWidget> ReticleWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
		class UReticleWidget* ReticleWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
		class UWidgetComponent* WidgetComponent;

    // Detect mouse over events on this component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		class UPrimitiveComponent* MouseOverComponent;

	UFUNCTION()
		void OnBeginCursorOver(UPrimitiveComponent* touchedComponent);

	UFUNCTION()
		void OnEndCursorOver(UPrimitiveComponent* touchedComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool isTarget;
	
};
