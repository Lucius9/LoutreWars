// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PaperSpriteComponent.h"
#include "Components/GridTileComponent.h"
#include "Components/GridMovementComponent.h"
#include "GridPawn.generated.h"

UCLASS()
class LOUTREWARS_API AGridPawn : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capsule")
	UCapsuleComponent *Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementComponent")
	UGridMovementComponent *MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpriteComponent")
	UPaperSpriteComponent *SpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow Component")
	UArrowComponent *Arrow;

	// Sets default values for this pawn's properties
	AGridPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;	

	bool IsMoving(); 
	
	UFUNCTION()
	void OnActorClick(AActor *Actor,FKey Key);

/*	UFUNCTION()
	void OnCapsuleClick(UPrimitiveComponent* pComponent, FKey inKey);

	AActor* OnActorTouch(ETouchIndex::Type FingerIndex, AActor *Actor);*/
};
