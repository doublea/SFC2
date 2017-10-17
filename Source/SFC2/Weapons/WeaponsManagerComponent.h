#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponsManagerComponent.generated.h"



/**
 * This class manages all weapon operations for a ShipPawn.
 * It contains descriptors, state, and visual components representing all
 * weapons on a ship.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFC2_API UWeaponsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponsManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    class AShipPawn* Ship;
	
};
