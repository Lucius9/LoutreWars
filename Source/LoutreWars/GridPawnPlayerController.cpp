// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "Components/GridMovementComponent.h"
#include "LoutreWarsGameMode.h"
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
	/*
	TODO pawn list
	*/
	for (TActorIterator<AGridPawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->MovementComponent->OnMovementEnd().AddUObject(this, &AGridPawnPlayerController::OnPawnMovementEnd);
	}
}

void AGridPawnPlayerController::SelectPawn(AGridPawn *GridPawn)
{
	if (!Busy && this->GetPawn()!=GridPawn)
	{
		Possess(GridPawn);
	}
}

void AGridPawnPlayerController::OnTileCursorOver(const UGridTileComponent &Tile)
{
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	if (IsValid(ControlledPawn) && !ControlledPawn->IsMoving() && Mode==EControllerMode::Movement)
	{
		UGridTileComponent *TilePointer=(UGridTileComponent *)&Tile;		
		if (ControlledPawn->MovementComponent->CreatePath2(TilePointer))
		{				
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
	if (IsValid(ControlledPawn) && Mode==EControllerMode::Movement)
	{
		UGridMovementComponent *MovementComponent=ControlledPawn->MovementComponent;
		UGridTileComponent *Location = Grid->GetTile(ControlledPawn->GetActorLocation());		
		if (Grid)
		{
			Grid->HideMovementRange();
			if (Location)
			{					
				TArray<UGridTileComponent *> InRange;
				Grid->TilesInMovementRange(Location, InRange, ControlledPawn, true);
				if (InRange.Contains(&Tile))
				{	
					Busy = true;
					Location->UnderCurrentPawn = false;
					MovementComponent->MoveTo((UGridTileComponent &)Tile);
					MovementComponent->HidePath();
					//UnPossess();
					EnableNavigationMode();
				}
			}			
		}
	}
}
void AGridPawnPlayerController::EnableMovementMode()
{
	Mode = EControllerMode::Movement;	
}

void AGridPawnPlayerController::EnableNavigationMode()
{	
	Mode = EControllerMode::Navigation;
}

void AGridPawnPlayerController::EnableAttackMode()
{
	Mode = EControllerMode::Attack;
}

void AGridPawnPlayerController::DisplayRange()
{
	AGridPawn *GridPawn = Cast<AGridPawn>(GetPawn());
	TActorIterator<ANavGrid>Itr(GetWorld());
	if (*Itr != NULL && GridPawn)
	{
		Itr->HideTilesInRange();
		UGridTileComponent* CurrentTile = Itr->GetTile(GridPawn->GetActorLocation());
		print(CurrentTile->GetOwner()->GetActorLabel());
		if (CurrentTile)
		{
			CurrentTile->UnderCurrentPawn = true;
			TArray<UGridTileComponent*> Out;
			Itr->ShowTilesInRange(CurrentTile, GridPawn);
		}
		else
		{
			print("CurrentTile NULL");
		}
	}
	else
	{
		print("Itr null");
	}
}

void AGridPawnPlayerController::OnPawnMovementEnd(AGridPawn &GridPawn)
{
	SelectPawn(&GridPawn);
	Busy = false;
	EnableEndMovementWidget();
}

void AGridPawnPlayerController::EnableMovementWidget()
{
	if (!Busy)
	{
		ALoutreWarsGameMode *GM = Cast<ALoutreWarsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GM->EnableMovementWidget();
	}	
}

void AGridPawnPlayerController::EnableEndMovementWidget()
{
	if (!Busy)
	{
		ALoutreWarsGameMode *GM = Cast<ALoutreWarsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GM->EnableEndMovementWidget();
	}	
}