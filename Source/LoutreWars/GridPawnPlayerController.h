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
	UGridTileComponent *FocusedTile;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="ControllerMode")
	bool Busy = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pawns")
	TArray<AGridPawn *> PawnsList;

public:
	AGridPawnPlayerController();
	virtual void BeginPlay()override;
	virtual bool CanSelectPawn(AGridPawn *GridPawn);
	virtual void SelectPawn(AGridPawn *GridPawn);	
	void OnTileTouched(const UGridTileComponent &Tile);
	void OnTileClicked(const UGridTileComponent &Tile);
	void OnPawnMovementEnd(AGridPawn &GridPawn);

protected : 
	void ChangeFocusedTile(UGridTileComponent *Tile);
	
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
	void EnableAttackWidget();

public :
	UFUNCTION(BlueprintCallable,Category="Movement")
	void DisplayMovementRange();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MovePawn();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CancelMovement();

public :
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DisplayEnemiesInRange();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CancelAttack();
};
