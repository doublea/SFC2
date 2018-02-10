// Fill out your copyright notice in the Description page of Project Settings.

#include "SFC2GameModeBase.h"
#include "SFCPlayerController.h"
#include "SFCGameState.h"
#include "ShipPawn.h"

ASFC2GameModeBase::ASFC2GameModeBase()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AShipPawn::StaticClass();

	PlayerControllerClass = ASFCPlayerController::StaticClass();

    GameStateClass = ASFCGameState::StaticClass();
}


