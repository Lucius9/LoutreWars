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

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Sprite")
	UPaperSprite *Sprite=NULL;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	UPaperFlipbookComponent *FlipbookComponent = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	UPaperFlipbook *Flipbook = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost = 1;

	// Sets default values for this actor's properties
	AGridTile();

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
