// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "NavGrid.h"
#include "GridPawnPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API AGridPawnPlayerController : public APlayerController
{
	GENERATED_BODY()

protected :
	ANavGrid *Grid;

public:
	AGridPawnPlayerController();

	virtual void BeginPlay()override;
	
	void OnTileCursorOver(const UGridTileComponent &Tile);
};
