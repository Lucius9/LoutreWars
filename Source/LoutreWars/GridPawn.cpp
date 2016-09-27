// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridPawn.h"


// Sets default values
AGridPawn::AGridPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

