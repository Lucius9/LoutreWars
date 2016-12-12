// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "Components/GridMovementComponent.h"
#include "LoutreWarsGameMode.h"
#include "GridPlayerState.h"
#include "GridPawnPlayerController.h"

//#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)


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
		Grid->OnTileTouched().AddUObject(this, &AGridPawnPlayerController::OnTileTouched);
		Grid->OnTileClicked().AddUObject(this, &AGridPawnPlayerController::OnTileClicked);
	}
	
	ULocalPlayer * LP = Cast<ULocalPlayer>(Player);
	if (LP)
	{		
		for (TActorIterator<AGridPawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{			
			if (ActorItr->AutoPossessAI==EAutoPossessAI::Disabled)
			{
				ActorItr->MovementComponent->OnMovementEnd().AddUObject(this, &AGridPawnPlayerController::OnPawnMovementEnd);
				PawnsList.Add(*ActorItr);
			}
		}
	}
}

bool AGridPawnPlayerController::CanSelectPawn(AGridPawn *GridPawn)
{
	return PawnsList.Contains(GridPawn);
}

void AGridPawnPlayerController::SelectPawn(AGridPawn *GridPawn)
{
	if (!Busy && this->GetPawn()!=GridPawn)
	{
		Possess(GridPawn);
	}
}

void AGridPawnPlayerController::OnTileTouched(const UGridTileComponent &Tile)
{
	ChangeFocusedTile((UGridTileComponent*)&Tile);
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	AGridPawn *GridPawn = Grid->GetPawn(FocusedTile);
	if (Mode == EControllerMode::Navigation && GridPawn != NULL)
	{
		if (CanSelectPawn(GridPawn))
		{
			SelectPawn(GridPawn);
			if (!GridPawn->MovementComponent->HasMoved && !GridPawn->HasPlayed)
			{
				//print("here");
				DisplayMovementRange();
				Mode = EControllerMode::Movement;
			}

		}
	}
	else if (Mode == EControllerMode::Movement && IsValid(ControlledPawn))
	{
		if (!ControlledPawn->MovementComponent->HasMoved && !ControlledPawn->HasPlayed)
		{
			EnableMovementWidget();
			ControlledPawn->MovementComponent->HidePath();
			if (ControlledPawn->MovementComponent->CreatePath(FocusedTile))
			{
				ControlledPawn->MovementComponent->ShowPath();
			}
			else
			{
				CancelMovement();
			}
		}
	}
	else if (Mode == EControllerMode::Attack && IsValid(ControlledPawn))
	{
		if (GridPawn && GridPawn != ControlledPawn && GridPawn->IsAttackableBy(ControlledPawn))
		{
			EnableAttackWidget();
		}
		if (!Tile.UnderCurrentPawn && Grid)
		{
			//print("here again");
			AGridPawn *Target = Grid->GetPawn((UGridTileComponent *)&Tile);
			if (Target && Target->IsAttackableBy(ControlledPawn))
			{
				//print("there");
				ControlledPawn->Attack(Target, true);
			}
		}
	}
}

void AGridPawnPlayerController::OnTileClicked(const UGridTileComponent &Tile)
{		
	ChangeFocusedTile((UGridTileComponent*)&Tile);
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	AGridPawn *GridPawn = Grid->GetPawn(FocusedTile);
	if (Mode == EControllerMode::Navigation && GridPawn!=NULL)
	{		
		if (CanSelectPawn(GridPawn))
		{
			SelectPawn(GridPawn);
			if (!GridPawn->MovementComponent->HasMoved && !GridPawn->HasPlayed)
			{
				//print("here");
				DisplayMovementRange();
				Mode = EControllerMode::Movement;
			}
			
		}		
	}
	else if (Mode == EControllerMode::Movement && IsValid(ControlledPawn))
	{
		if (!ControlledPawn->MovementComponent->HasMoved && !ControlledPawn->HasPlayed)
		{
			EnableMovementWidget();
			ControlledPawn->MovementComponent->HidePath();
			if (ControlledPawn->MovementComponent->CreatePath(FocusedTile))
			{				
				ControlledPawn->MovementComponent->ShowPath();
			}
			else
			{					
				CancelMovement();
			}
		}				
	}
	else if (Mode == EControllerMode::Attack && IsValid(ControlledPawn))
	{		
		if (GridPawn && GridPawn != ControlledPawn && GridPawn->IsAttackableBy(ControlledPawn))
		{
			EnableAttackWidget();
		}
		if (!Tile.UnderCurrentPawn && Grid)
		{
			//print("here again");
			AGridPawn *Target = Grid->GetPawn((UGridTileComponent *)&Tile);
			if (Target && Target->IsAttackableBy(ControlledPawn))
			{
				//print("there");
				ControlledPawn->Attack(Target, true);
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

void AGridPawnPlayerController::DisplayMovementRange()
{	
	AGridPawn *GridPawn = Cast<AGridPawn>(GetPawn());	
	if (Grid != NULL && GridPawn)
	{
		Grid->HideHighlightedTiles();
		UGridTileComponent* CurrentTile = Grid->GetTile(GridPawn->GetActorLocation());	
		if (CurrentTile)
		{
			CurrentTile->UnderCurrentPawn = true;		
			Grid->ShowMovementRange(CurrentTile, GridPawn);
		}
		else
		{
			//print("CurrentTile NULL");
		}
	}
	else
	{
		//print("Itr null");
	}
}

void AGridPawnPlayerController::DisplayEnemiesInRange()
{
	AGridPawn *GridPawn = Cast<AGridPawn>(GetPawn());
	TActorIterator<ANavGrid>Itr(GetWorld());
	if (*Itr != NULL && GridPawn)
	{
		Itr->HideHighlightedTiles();
		UGridTileComponent *CurrentTile = Itr->GetTile(GridPawn->GetActorLocation());
		if (CurrentTile)
		{
			CurrentTile->UnderCurrentPawn = true;
			Itr->ShowEnnemiesTileInRange(GridPawn);
		}

	}
}

void AGridPawnPlayerController::OnPawnMovementEnd(AGridPawn &GridPawn)
{
	SelectPawn(&GridPawn);
	Busy = false;
	EnableEndMovementWidget();
}

void AGridPawnPlayerController::ChangeFocusedTile(UGridTileComponent *Tile)
{
	ALoutreWarsGameMode *GM = Cast<ALoutreWarsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (FocusedTile == NULL)
	{
		AGridTile *NewTile = Cast<AGridTile>(Tile->GetOwner());
		FocusedTile = Tile;
		if (NewTile)
		{
			NewTile->EnableCurrentTileOverlay();
			if (GM)
			{					
				GM->UpdateTileWidget();
				AGridPawn *GridPawn = Grid->GetPawn(FocusedTile);
				if (GridPawn)
				{
					GM->UpdatePawnWidget();
				}
				else
				{
					GM->HidePawnWidget();
				}
			}
		}
	}
	else if (FocusedTile != NULL && FocusedTile!=Tile)
	{
		AGridTile *PreviousTile = Cast<AGridTile>(FocusedTile->GetOwner());		
		AGridTile *NewTile = Cast<AGridTile>(Tile->GetOwner());
		FocusedTile = Tile;
		if (PreviousTile && NewTile)
		{
			PreviousTile->DisableCurrentTileOverlay();
			NewTile->EnableCurrentTileOverlay();		
			if (GM)
			{
				GM->UpdateTileWidget();
				AGridPawn *GridPawn = Grid->GetPawn(FocusedTile);
				if (GridPawn )
				{
					GM->UpdatePawnWidget();
				}
				else
				{
					GM->HidePawnWidget();
				}
			}
		}
	}	
}

void AGridPawnPlayerController::EnableMovementWidget()
{
	if (!Busy)
	{	
		EnableMovementMode();
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

void AGridPawnPlayerController::EnableAttackWidget()
{
	if (!Busy)
	{
		ALoutreWarsGameMode *GM = Cast<ALoutreWarsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GM->EnableAttackWidget();
	}
}

void AGridPawnPlayerController::MovePawn()
{
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	if (ControlledPawn)
	{
		UGridTileComponent* CurrentTile = Grid->GetTile(ControlledPawn->GetActorLocation());
		CurrentTile->UnderCurrentPawn = false;
		Busy = true;		
		ControlledPawn->MovementComponent->HidePath();
		Grid->HideHighlightedTiles();
		ControlledPawn->MovementComponent->FollowPath();		
	}
}

void AGridPawnPlayerController::CancelMovement()
{
	Grid->HideHighlightedTiles();
	ALoutreWarsGameMode *GM = Cast<ALoutreWarsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->HideWidget();
	Mode = EControllerMode::Navigation;
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	if (ControlledPawn)
	{
		ControlledPawn->MovementComponent->HidePath();
	}
}

void AGridPawnPlayerController::Attack()
{
	AGridPawn *ControlledPawn = Cast<AGridPawn>(GetPawn());
	AGridPawn *Target = Grid->GetPawn(FocusedTile);

	if (Target && ControlledPawn)
	{
		ControlledPawn->Attack(Target,true);
	}
}

void AGridPawnPlayerController::CancelAttack()
{
	Grid->HideHighlightedTiles();
	UGridTileComponent *Tile = Grid->GetTile(GetPawn()->GetActorLocation());
	if(Tile)
	{
		ChangeFocusedTile(Tile);
		EnableEndMovementWidget();
	}
}

AGridPawn * AGridPawnPlayerController::GetPawnOnFocusedTile()
{
	if (FocusedTile)
	{
		return Grid->GetPawn(FocusedTile);
	}
	return NULL;
}