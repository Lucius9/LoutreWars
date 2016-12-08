// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/SplineComponent.h"
#include "GridTileComponent.generated.h"

UENUM(BlueprintType)
enum class EGridMovementMode : uint8
{
	Walking				UMETA(DisplayName = "Walking"),
	Driving				UMETA(DisplayName = "Driving"),
	Flying				UMETA(DisplayName = "Flying"),
	Swimming			UMETA(DisplayName = "Swimming")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOUTREWARS_API UGridTileComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent *Extent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Attributes")
	FString GroundName ="Plaine";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Attributes")
	float Cost=1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Attributes")
	float Defense = 1.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Neighbours")
	TArray<UGridTileComponent*> Neighbours;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent *PawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Attributes")
	TArray<EGridMovementMode> TileMovementModes;

	/* Distance from starting point of path */
	float Distance;
	/* Previous tile in path */
	UGridTileComponent *Backpointer = NULL;
	/* Is this node in the 'visited' set? - Helper var for pathfinding */
	bool Visited;

	bool UnderCurrentPawn = false;

protected:
	class ANavGrid *Grid;
	int FScore, HScore;
	

public:
	// Sets default values for this component's properties
	UGridTileComponent(const FObjectInitializer &ObjectInitializer);
	// Set values when the component is added to an Actor
	virtual void OnComponentCreated() override;	
	virtual void BeginPlay() override;

	float GetCost();
	void SetCost(float cost);	
	void ComputeNeighbours();
	TArray<UGridTileComponent*>* GetNeighbours();
	
public :
	FVector GetPawnLocation();
	virtual bool Traversable(class AGridPawn *GridPawn);
	/* is there anything blocking an actor from moving from FromPos to this tile? Uses the capsule for collision testing */
	virtual bool Obstructed(const FVector &FromPos, const UCapsuleComponent &CollisionCapsule);
	/* is there anything blocking an actor from moving between From and To? Uses the capsule for collision testing */
	bool static Obstructed(const FVector &From, const FVector &To, const UCapsuleComponent &CollisionCapsule);
	/* Return the neighbours that are not Obstructed() */
	virtual void GetUnobstructedNeighbours(const UCapsuleComponent &CollisionCapsule, TArray<UGridTileComponent *> &OutNeighbours);
	virtual void GetTraversableNeighbours(class AGridPawn *GridPawn,TArray<UGridTileComponent *> &OutNeighbours);
	/* Reset variables used in pathfinding */
	virtual void ResetPath();	

public:
	
	void ComputeHScore(UGridTileComponent *To);
	int GetHScore();
	int GetFScore();

public : 
	UFUNCTION()
	void OnTileTouched(ETouchIndex::Type FingerIndex, UPrimitiveComponent *TouchedComponent);

	UFUNCTION()
	void OnTileCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnEndTileCursorOver(UPrimitiveComponent *TouchedComponent);

	UFUNCTION()
	void OnTileClicked(UPrimitiveComponent* TouchedComponent, FKey Key);

public :
	void AddSplinePoint(USplineComponent &Out);
};