// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "Components/GridTileComponent.h"
#include "GridMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API UGridMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement") float MovementRange = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float MaxWalkAngle = 90.0f;

protected:
	UGridTileComponent *CurrentTile;
	TArray<UGridTileComponent *>CurrentPath;
	class ANavGrid *Grid;
public:

	virtual void BeginPlay() override;

	UGridTileComponent* GetCurrentTile();
	void SetCurrentTile(UGridTileComponent *Tile);

	bool CreatePath(UGridTileComponent &Target);	
	void ShowPath();
	void HidePath();
};
