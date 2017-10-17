// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetManager.generated.h"

/**
 * Manages the targeting state machine.
 * 
 * Actors and components that want to be targeted should listen for
 * TargetSelectedEvent and call NewTargetAcquired in the handler if
 * they wish to become the new target.
 *
 * Objects wanting to react to target changes (e.g. the HUD) should subscribe
 * to TargetAcquiredEvent.
 */
UCLASS()
class SFC2_API UTargetManager : public UObject
{
	GENERATED_BODY()

public:
	UTargetManager();

    // TargetSelectedEvent is fired if the player attempted to target (e.g.
    // right-click) an AActor. Listeners need to check if the AActor is a
    // target they want to react to and maybe call NewTargetAcquired() if they
    // want to make the new AActor the current target.
	DECLARE_EVENT_OneParam(UTargetManager, FTargetSelectedEvent, AActor*);
	FTargetSelectedEvent& OnTargetSelected() { return TargetSelectedEvent; }

    // TargetAcquiredEvent is fired when someone calls NewTargetAcuired. This
    // means a new AActor has been set as the current target.
	DECLARE_EVENT_OneParam(UTargetManager, FTargetAcquiredEvent, AActor*);
	FTargetAcquiredEvent& OnTargetAcquired() { return TargetAcquiredEvent; }

    // Meant to be called from PlayerController. This signals that the player
    // attempted to select a new target. It will broadcast the
    // TargetSelectedEvent to all listeners. If a listener accepts the target
    // request, it will call NewTargetAcquired.
    UFUNCTION(BlueprintCallable)
    void NewTargetSelected(AActor* target);

    // Meant to be called by listeners of TargetSelectedEvent (e.g.
    // TargetComponent) when they've identified themselves as the target passed
    // in the event and wish to accept being targetted.
    // TargetManager can reject a listener from becoming the target by
    // returning false here.
    UFUNCTION(BlueprintCallable)
    bool NewTargetAcquired(AActor* target);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    AActor* CurrentTarget;
    AActor* GetCurrentTarget() const { return CurrentTarget; }
    bool HasTarget() const { return CurrentTarget != nullptr; }

private:
	FTargetSelectedEvent TargetSelectedEvent;
	FTargetAcquiredEvent TargetAcquiredEvent;
};
