// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "NavGrid.h"
#include "GridPawn.h"
#include "Tiles/GridTile.h"
#include "GridMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

void UGridMovementComponent::BeginPlay()
{	
	TActorIterator<ANavGrid> Itr(GetWorld());
	if (Itr)
	{
		Grid = *Itr;
	}
}
UGridTileComponent* UGridMovementComponent::GetCurrentTile()
{
	return CurrentTile;
}

void UGridMovementComponent::SetCurrentTile(UGridTileComponent *Tile)
{
	CurrentTile = Tile;
}

bool UGridMovementComponent::CreatePath(UGridTileComponent &Target)
{	
	CurrentPath.Empty();
	AGridPawn *Pawn = Cast<AGridPawn>(GetOwner());
	CurrentTile = Grid->GetTile(Pawn->GetActorLocation());	
	if (CurrentTile != NULL )
	{
		TArray<UGridTileComponent*> Range;
		Grid->TilesInRange(CurrentTile, Range, Pawn, true);		
		if (Range.Contains(&Target))
		{			
			UGridTileComponent *Current = &Target;
			while (Current)
			{				
				CurrentPath.Add(Current);
				Current = Current->Backpointer;
			}		
			Algo::Reverse(CurrentPath);			
			return true;
		}
	}
	else
	{
		print("Create Path CurrentTile NULL");
	}
	print("false");
	return false;
}

void UGridMovementComponent::ShowPath()
{
	print(FString::FromInt(CurrentPath.Num()));
	if (CurrentPath.Num() >= 1)
	for (int i = 1; i < CurrentPath.Num();++i)
	{
		AActor *Actor=CurrentPath[i]->GetOwner();
		AGridTile *TileActor = Cast<AGridTile>(Actor);
		if (IsValid(TileActor))
		{
			TileActor->EnablePathOverlay();
		}		
	}
}

void UGridMovementComponent::HidePath()
{
	if (CurrentPath.Num() != 0)
	{
		for (UGridTileComponent *T : CurrentPath)
		{
			AActor *Actor = T->GetOwner();
			AGridTile *TileActor = Cast<AGridTile>(Actor);
			if (IsValid(TileActor))
			{
				TileActor->DisablePathOverlay();
			}
		}
	}
}
