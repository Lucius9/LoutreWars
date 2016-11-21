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

void ALoutreWarsGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ALoutreWarsGameMode::EnableMovementWidget()
{
	ChangeMenuWidget(MovementWidgetClass);
}

void ALoutreWarsGameMode::EnableEndMovementWidget()
{
	ChangeMenuWidget(EndMovementWidgetClass);
}
