// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridPawn.h"
#include "GridPawnPlayerController.h"
#include "GridPlayerState.h"
#include "LoutreWarsGameMode.h"


ALoutreWarsGameMode::ALoutreWarsGameMode()
{
	//DefaultPawnClass = AGridPawn::StaticClass();
	PlayerControllerClass = AGridPawnPlayerController::StaticClass();

	PlayerStateClass = AGridPlayerState::StaticClass();
}

