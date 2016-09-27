// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/GridTileComponent.h"
#include "NavGrid.generated.h"

UCLASS()
class LOUTREWARS_API ANavGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavGrid();

	virtual void OnConstruction(const FTransform &Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
	void GetAllTiles(TArray<UGridTileComponent*> &Out, UWorld *World);
	
};
