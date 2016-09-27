// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "GridTileComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOUTREWARS_API UGridTileComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent *Extent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Cost;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="ContactPoints")
	TArray<FVector> ContactPoints;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Neighbours")
	TArray<UGridTileComponent*> Neighbours;

	

public:
	// Sets default values for this component's properties
	UGridTileComponent(const FObjectInitializer &ObjectInitializer);

	// Set values when the component is added to an Actor
	virtual void OnComponentCreated() override;	
	float GetCost();
	void SetCost(float cost);
	TArray<FVector>* GetContactPoints();
	TArray<UGridTileComponent*>* GetNeighbours();

protected :
	void ComputeContactPoints();
	void ComputeNeighbours();
	
public :
	virtual bool Traversable(float MaxWalkAngle);
	/* is there anything blocking an actor from moving from FromPos to this tile? Uses the capsule for collision testing */
	virtual bool Obstructed(const FVector &FromPos, const UCapsuleComponent &CollisionCapsule);
	/* is there anything blocking an actor from moving between From and To? Uses the capsule for collision testing */
	bool static Obstructed(const FVector &From, const FVector &To, const UCapsuleComponent &CollisionCapsule);
	/* Return the neighbours that are not Obstructed() */
	virtual void GetUnobstructedNeighbours(const UCapsuleComponent &CollisionCapsule, TArray<UGridTileComponent *> &OutNeighbours);	
};