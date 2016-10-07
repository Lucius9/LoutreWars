// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridPawnPlayerController.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)


AGridPawnPlayerController::AGridPawnPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	DefaultClickTraceChannel = ECC_Visibility;
}

void AGridPawnPlayerController::BeginPlay()
{
	TActorIterator<ANavGrid> Itr(GetWorld());
	if (Itr)
	{
		Grid = *Itr;
		Grid->OnTileCursorOver().AddUObject(this, &AGridPawnPlayerController::OnTileCursorOver);
		Grid->OnEndTileCursorOver().AddUObject(this, &AGridPawnPlayerController::OnEndTileCursorOver);
	}

}

void AGridPawnPlayerController::OnTileCursorOver(const UGridTileComponent &Tile)
{
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	if (IsValid(ControlledPawn))
	{
		print("valid control pawn");
		if (ControlledPawn->MovementComponent->CreatePath((UGridTileComponent &)Tile))
		{
			print("here");
			ControlledPawn->MovementComponent->ShowPath();
		}
	}
}

void AGridPawnPlayerController::OnEndTileCursorOver(const UGridTileComponent &Tile)
{
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	if (IsValid(ControlledPawn))
	{
		ControlledPawn->MovementComponent->HidePath();
	}
}