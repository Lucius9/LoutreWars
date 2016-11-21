// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridPawn.h"
#include "Tiles/GridTile.h"
#include "NavGrid.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

ECollisionChannel ANavGrid::ECC_Tile = ECollisionChannel::ECC_GameTraceChannel1; 

// Sets default values
ANavGrid::ANavGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent *Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

}

ANavGrid * ANavGrid::GetNavGrid(UWorld *World)
{
	TActorIterator<ANavGrid> Itr(World, ANavGrid::StaticClass());
	if (Itr)
	{
		return *Itr;
	}
	else
	{
		return NULL;
	}
}

void ANavGrid::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void ANavGrid::BeginPlay()
{
	Super::BeginPlay();
	TArray<UGridTileComponent*> Tiles;
	GetAllTiles(Tiles, GetWorld());
	for (UGridTileComponent* Tile :Tiles)
	{		
		Tile->ComputeNeighbours();
	}
}

// Called every frame
void ANavGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ANavGrid::GetAllTiles(TArray<UGridTileComponent*> &Out, UWorld *World)
{
	
	for (TObjectIterator<UGridTileComponent> Itr; Itr;++Itr)
	{
		if (Itr->GetWorld() == World)
		{
			Out.Add(*Itr);
		}
	}
}

void ANavGrid::TilesInMovementRange(UGridTileComponent * Tile, TArray<UGridTileComponent*>& OutArray, AGridPawn *Pawn, bool DoCollisionTests)
{
	OutArray.Empty();
	TArray<UGridTileComponent*> AllTiles;
	GetAllTiles(AllTiles, GetWorld());
	for (UGridTileComponent *T : AllTiles)
	{
		T->ResetPath();
	}	
	UGridTileComponent *Current = Tile;
	Current->Distance = 0;

	TArray<UGridTileComponent*> Neighbours=*Current->GetNeighbours();	
	//Current->GetUnobstructedNeighbours(*Pawn->Capsule, Neighbours);

	TArray<UGridTileComponent*> CheckList;
	while(Current)
	{		
		Neighbours = *Current->GetNeighbours();
		FVector CurrentPawnLocation = Current->PawnLocation->GetComponentLocation();
		//Current->GetUnobstructedNeighbours(*Pawn->Capsule, Neighbours);		
		for (UGridTileComponent *Neighbour : Neighbours)
		{	
			FVector NeighbourPawnLocation = Neighbour->PawnLocation->GetComponentLocation();		
			
			float Distance = Neighbour->Cost + Current->Distance;
			if (!Neighbour->Visited && Distance <= Pawn->MovementComponent->MovementRange)
			{
				Neighbour->Backpointer = Current;
				Neighbour->Distance = Distance;
				CheckList.AddUnique(Neighbour);
			}
								
		}
		Current->Visited = true;
		CheckList.Remove(Current);		
		if (Current != Tile && !Current->Obstructed(CurrentPawnLocation,*Pawn->Capsule))
		{			
			OutArray.Add(Current);
		}
		if (CheckList.Num()>0)
		{			
			Current = CheckList[0];
		}
		else
		{			
			Current = NULL;
		}
	}
}

void ANavGrid::ShowMovementRange(UGridTileComponent *Tile, AGridPawn *Pawn)
{
	HighlightedTiles.Empty();
	TArray<UGridTileComponent*>Range;
	TilesInMovementRange(Tile, Range, Pawn, true);
	for (UGridTileComponent*T : Range)
	{
		AActor *Actor = T->GetOwner();
		AGridTile *TileActor = Cast<AGridTile>(Actor);
		if (IsValid(TileActor))
		{
			TileActor->EnableRangeOverlay();
			HighlightedTiles.Add(TileActor);
		}
	}
}

void ANavGrid::HideMovementRange()
{
	for (AGridTile *T : HighlightedTiles)
	{
		T->DisableRangeOverlay();
	}
}

UGridTileComponent * ANavGrid::LineTraceTile(const FVector &Start, const FVector &End)
{
	FHitResult HitResult;
	TArray<FHitResult> HitResults;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ANavGrid::ECC_Tile);		
	UPrimitiveComponent *Comp = HitResult.GetComponent();
	UGridTileComponent *Tile = Cast<UGridTileComponent>(Comp);
	if (IsValid(Tile))
	{		
		return Tile;
	}
	else if(Comp)
	{
		TArray<USceneComponent *> Components;
		Comp->GetParentComponents(Components);
		for (auto *C : Components)
		{
			UGridTileComponent *T = Cast<UGridTileComponent>(C);
			if (T) 
			{				
				return T; 
			}
		}		
	}
	return NULL;
}

AGridPawn * ANavGrid::LineTracePawn(const FVector &Start,const FVector &End)
{
	FHitResult HitResult;
	TArray<FHitResult> HitResults;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,ECollisionChannel::ECC_Pawn);
	AActor *Actor = HitResult.GetActor();
	AGridPawn *Pawn = Cast<AGridPawn>(Actor);
	if (IsValid(Pawn))
	{		
		return Pawn;
	}	
	return NULL;
}


void ANavGrid::GetEnnemiesInRange(AGridPawn *Pawn, TArray<AGridPawn*> &Out)
{
	Out.Empty();
	TArray<UGridTileComponent *> AttackRange;
	TilesInAttackRange(Pawn, AttackRange);
	for (UGridTileComponent *N : AttackRange)
	{		
		AGridPawn *Target = GetPawn(N);			
		if (Target && Target->IsAttackableBy(Pawn))
		{
			Out.Add(Target);
		}
	}
}

UGridTileComponent* ANavGrid::GetTile(const FVector &Position)
{	
	return LineTraceTile(Position - FVector(0.0f, 10.0f, 0.0f), Position + FVector(0.0f, 10.0f, 0.0f));
}

AGridPawn * ANavGrid::GetPawn(UGridTileComponent *Tile)
{
	return LineTracePawn(Tile->GetComponentLocation(), Tile->PawnLocation->GetComponentLocation());
}

void ANavGrid::ResetTiles()
{
	TArray<UGridTileComponent *>Tiles;
	GetAllTiles(Tiles, GetWorld());
	for (UGridTileComponent *N : Tiles)
	{
		N->ResetPath();
	}
}

void ANavGrid::TilesInAttackRange(AGridPawn *Pawn, TArray<UGridTileComponent*> &Out)
{
	ResetTiles();
	Out.Empty();
	UGridTileComponent *Current = GetTile(Pawn->GetActorLocation());	
	TArray<UGridTileComponent *>CheckList;
	Current->Distance = 0;	
	while (Current)
	{		
		for (UGridTileComponent *N : *(Current->GetNeighbours()))
		{		
			if (!N->Visited)
			{				
				N->Backpointer = Current;
				N->Distance = Current->Distance + 1;
				if (N->Distance >= Pawn->AttackRangeMin && N->Distance <= Pawn->AttackRangeMax)
				{
					Out.Add(N);
					CheckList.Add(N);
				}
			}
			
		}	
		Current->Visited = true;
		CheckList.Remove(Current);
		Current = NULL;
		if (CheckList.Num() > 0)
		{
			Current = CheckList[0];
		}		
	}		
}

void ANavGrid::TileCursorOver(UGridTileComponent &Tile)
{	
	OnTileCursorOverEvent.Broadcast(Tile);
}

void ANavGrid::EndTileCursorOver(UGridTileComponent &Tile)
{
	OnEndTileCursorOverEvent.Broadcast(Tile);
}

void ANavGrid::TileClicked(UGridTileComponent &Tile)
{
	OnTileClickedEvent.Broadcast(Tile);
}

