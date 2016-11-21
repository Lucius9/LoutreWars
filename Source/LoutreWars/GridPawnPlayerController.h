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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="ControllerMode")
	bool Busy = false;

public:
	AGridPawnPlayerController();

	virtual void BeginPlay()override;
	virtual void SelectPawn(AGridPawn *GridPawn);
	void OnTileCursorOver(const UGridTileComponent &Tile);
	void OnEndTileCursorOver(const UGridTileComponent &Tile);
	void OnTileClicked(const UGridTileComponent &Tile);
	void OnPawnMovementEnd(AGridPawn &GridPawn);
	
public :
	UFUNCTION(BlueprintCallable, Category = "ControllerMode")
	void EnableMovementMode();
	UFUNCTION(BlueprintCallable, Category = "ControllerMode")
	void EnableNavigationMode();
	UFUNCTION(BlueprintCallable, Category = "ControllerMode")
	void EnableAttackMode();

public :
	void EnableMovementWidget();
	void EnableEndMovementWidget();

public :
	UFUNCTION(BlueprintCallable,Category="Movement")
	void DisplayRange();

};
