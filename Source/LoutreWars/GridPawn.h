// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PaperSpriteComponent.h"
#include "Components/GridTileComponent.h"
#include "Components/GridMovementComponent.h"
#include "Faction/Faction.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int AttackRangeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int AttackRangeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	FString PawnName = "GridPawn";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	int PV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	int Unit;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Pawn Turn")
	bool HasPlayed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Turn")
	bool HasMoved = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	int UnitPv;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	int Atc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	int AtcSpe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	int Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stat")
	int DefSpe;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	AFaction *Faction;

	// Sets default values for this pawn's properties
	AGridPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;	

	bool IsMoving(); 
	
	UFUNCTION()
	void OnActorClick(AActor *Actor,FKey Key);

	UFUNCTION()
	void OnActorTouched(ETouchIndex::Type FingerIndex, AActor *Actor);

	virtual bool IsAttackableBy(AGridPawn *Attacker);

	virtual void Attack(AGridPawn *Attacker, bool Attackable);

	virtual void BeginTurn();

/*	UFUNCTION()
	void OnCapsuleClick(UPrimitiveComponent* pComponent, FKey inKey);

	AActor* OnActorTouch(ETouchIndex::Type FingerIndex, AActor *Actor);*/
};
