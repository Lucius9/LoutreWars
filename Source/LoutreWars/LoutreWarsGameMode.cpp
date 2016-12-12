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

void ALoutreWarsGameMode::BeginPlay()
{
	Super::BeginPlay();
	EnableNavigationWidget();
}

void ALoutreWarsGameMode::ChangeActionWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentActionWidget != nullptr)
	{
		CurrentActionWidget->RemoveFromViewport();
		CurrentActionWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentActionWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);		
		if (CurrentActionWidget != nullptr)
		{
			CurrentActionWidget->AddToViewport();
		}
	}
}

void ALoutreWarsGameMode::HideWidget()
{
	ChangeActionWidget(NULL);
}

void ALoutreWarsGameMode::EnableMovementWidget()
{
	ChangeActionWidget(MovementWidgetClass);
}

void ALoutreWarsGameMode::EnableEndMovementWidget()
{
	ChangeActionWidget(EndMovementWidgetClass);
}

void ALoutreWarsGameMode::EnableAttackWidget()
{
	ChangeActionWidget(AttackWidgetClass);
}


void ALoutreWarsGameMode::EnableNavigationWidget()
{
	if (NavigationWidget == nullptr)
	{
		NavigationWidget = CreateWidget<UNavigationWidget>(GetWorld(), NavigationWidgetClass);
		NavigationWidget->AddToViewport();
	}
}
void ALoutreWarsGameMode::DisableNavigationWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->RemoveFromViewport();
		NavigationWidget = nullptr;
	}
}

void ALoutreWarsGameMode::UpdateTileWidget()
{
	if (NavigationWidget != nullptr)
	{
		//NavigationWidget->HideTileWidget();
		NavigationWidget->UpdateTileWidget();
	}
}

void ALoutreWarsGameMode::UpdatePawnWidget()
{
	if (NavigationWidget != nullptr)
	{		
		NavigationWidget->UpdatePawnWidget();
	}

}
void ALoutreWarsGameMode::HidePawnWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->HidePawnWidget();
	}
}
