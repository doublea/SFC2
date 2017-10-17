// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetManager.h"
#include "Camera/CameraComponent.h"

UTargetManager::UTargetManager()
{
}

void UTargetManager::NewTargetSelected(AActor* target) {
    TargetSelectedEvent.Broadcast(target);
}

bool UTargetManager::NewTargetAcquired(AActor* target) {
    CurrentTarget = target;
    TargetAcquiredEvent.Broadcast(CurrentTarget);
    // TODO: Do some locking and make sure that only one NewTargetAcquired call
    // returns true per TargetSelectedEvent.
    return true;
}
