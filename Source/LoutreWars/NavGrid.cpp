// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "NavGrid.h"


// Sets default values
ANavGrid::ANavGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent *Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

}

void ANavGrid::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void ANavGrid::BeginPlay()
{
	Super::BeginPlay();
	
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

