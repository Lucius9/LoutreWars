// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/GridTileComponent.h"
#include "GridTile.generated.h"

UCLASS()
class LOUTREWARS_API AGridTile : public AActor
{
	GENERATED_BODY()
protected:
	
	
public:		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UGridTileComponent *GridTileComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	UPaperSpriteComponent *SpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	UPaperSpriteComponent *FocusOverlaySpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	UPaperSpriteComponent *BottomOverlaySpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	UPaperSpriteComponent *TopOverlaySpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlays")
	UPaperSprite *MovementRangeOverlaySprite = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlays")
	UPaperSprite *AttackRangeOverlaySprite = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlays")
	UPaperSprite *AttackOverlaySprite = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Overlays")
	UPaperSprite *PathOverlaySprite=NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlays")
	UPaperSprite *CurrentTileOverlaySprite;
	
	// Sets default values for this actor's properties
	AGridTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void EnableCurrentTileOverlay();
	void DisableCurrentTileOverlay();

	void EnableMovementRangeOverlay();
	void DisableMovementRangeOverlay();

	void EnablePathOverlay();
	void DisablePathOverlay();

	void EnableAttackRangeOverlay();
	void DisableAttackRangeOverlay();

	void EnableAttackOverlay();
	void DisableAttackOverlay();
};