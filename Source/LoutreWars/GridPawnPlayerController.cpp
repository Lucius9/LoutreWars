// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "Components/GridMovementComponent.h"
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
		Grid->OnTileClicked().AddUObject(this, &AGridPawnPlayerController::OnTileClicked);
	}

}

void AGridPawnPlayerController::OnTileCursorOver(const UGridTileComponent &Tile)
{
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	if (IsValid(ControlledPawn) && !ControlledPawn->IsMoving() && Mode==EControllerMode::Movement)
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
	if (IsValid(ControlledPawn) && Mode == EControllerMode::Movement)
	{
		ControlledPawn->MovementComponent->HidePath();
	}
}

void AGridPawnPlayerController::OnTileClicked(const UGridTileComponent &Tile)
{	
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	if (IsValid(ControlledPawn))
	{
		UGridMovementComponent *MovementComponent=ControlledPawn->MovementComponent;
		UGridTileComponent *Location = Grid->GetTile(ControlledPawn->GetActorLocation());		
		if (Grid)
		{
			Grid->HideTilesInRange();
			if (Location)
			{
				print("grille et tile valide");				
				TArray<UGridTileComponent *> InRange;
				Grid->TilesInRange(Location, InRange, ControlledPawn, true);
				if (InRange.Contains(&Tile))
				{
					print("je bouge");
					Location->UnderCurrentPawn = false;
					MovementComponent->MoveTo((UGridTileComponent &)Tile);
					MovementComponent->HidePath();
					EnableNavigationMode();
				}
			}			
			else
			{
				print("tile not in range");
			}
		}
	}
}
void AGridPawnPlayerController::EnableMovementMode()
{
	print("J'active le mode mouvement");
	Mode = EControllerMode::Movement;
}

void AGridPawnPlayerController::EnableNavigationMode()
{
	print("Je retourne en mode navigation");
	Mode = EControllerMode::Navigation;
}

void AGridPawnPlayerController::EnableAttackMode()
{
	Mode = EControllerMode::Attack;
}