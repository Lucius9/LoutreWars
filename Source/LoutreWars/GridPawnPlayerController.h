// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "NavGrid.h"
#include "GridPawnPlayerController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EControllerMode : uint8
{
	Navigation			UMETA(DisplayName = "Navigation"),
	Movement			UMETA(DisplayName = "Movement"),
	Attack				UMETA(DisplayName = "Attack"),	
};

UCLASS()
class LOUTREWARS_API AGridPawnPlayerController : public APlayerController
{
	GENERATED_BODY()

protected :
	ANavGrid *Grid;
	EControllerMode Mode=EControllerMode::Navigation;

public:
	AGridPawnPlayerController();

	virtual void BeginPlay()override;
	
	void OnTileCursorOver(const UGridTileComponent &Tile);
	void OnEndTileCursorOver(const UGridTileComponent &Tile);
	void OnTileClicked(const UGridTileComponent &Tile);
	
public :
	void EnableMovementMode();
	void EnableNavigationMode();
	void EnableAttackMode();

};
