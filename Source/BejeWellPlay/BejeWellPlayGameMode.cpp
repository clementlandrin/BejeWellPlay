// Copyright Epic Games, Inc. All Rights Reserved.

#include "BejeWellPlayGameMode.h"
#include "BejeWellPlayPlayerController.h"
#include "BejeWellPlayPawn.h"

ABejeWellPlayGameMode::ABejeWellPlayGameMode()
{
	// no pawn by default
	DefaultPawnClass = ABejeWellPlayPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ABejeWellPlayPlayerController::StaticClass();
}
