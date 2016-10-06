// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/GridTileComponent.h"
#include "GridPawn.h"
#include "NavGrid.generated.h"

UCLASS()
class LOUTREWARS_API ANavGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavGrid();

	static ANavGrid *GetNavGrid(UWorld *World);

	virtual void OnConstruction(const FTransform &Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
	void GetAllTiles(TArray<UGridTileComponent*> &Out, UWorld *World);
	UGridTileComponent * LineTraceTile(const FVector &Start, const FVector &End);

public :
	UGridTileComponent *GetTile(const FVector &Position);
	virtual void TilesInRange(UGridTileComponent * Tile, TArray<UGridTileComponent*>& OutArray, AGridPawn *Pawn, bool DoCollisionTests);
	
	static ECollisionChannel ECC_Tile;

public:
	void TileCursorOver(UGridTileComponent &Tile);

/*Events Delegates*/
public:
	DECLARE_EVENT_OneParam(ANavGrid, FOnTileCursorOver, const UGridTileComponent&);

	FOnTileCursorOver& OnTileCursorOver() { return OnTileCursorOverEvent; }

private:
	FOnTileCursorOver OnTileCursorOverEvent;
	
};
