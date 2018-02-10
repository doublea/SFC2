// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SFCGameState.generated.h"

/**
 * 
 */
UCLASS()
class SFC2_API ASFCGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
    /** How many seconds is a "turn" **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GameSpeed = 8.0f;
};
