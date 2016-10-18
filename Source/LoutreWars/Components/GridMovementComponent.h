// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "Components/GridTileComponent.h"
#include "Components/SplineComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float MaxWalkingSpeed = 5.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement") float MaxRotationSpeed = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float MaxWalkAngle = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool bUseRootMotion = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualization")
	USplineComponent *Spline;

protected:
	UGridTileComponent *CurrentTile;
	TArray<UGridTileComponent *>CurrentPath;
	class ANavGrid *Grid;
	bool Moving=false;
	float Distance = 0;


public:
	UGridMovementComponent(const FObjectInitializer &ObjectInitializer);
	virtual void BeginPlay() override;	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UGridTileComponent* GetCurrentTile();
	void SetCurrentTile(UGridTileComponent *Tile);
	bool IsMoving();

	//Pathing
public :
	bool CreatePath(UGridTileComponent &Target);	
	void ShowPath();
	void HidePath();
	bool MoveTo(UGridTileComponent &Target);
	void FollowPath();
	void StopMoving();

protected :	
	FRotator ComputeRotator(const FVector &PawnLocation, const FVector &Target);
	FRotator LimitRotation(const FRotator &OldRotator, const FRotator &NewRotator, float DeltaSeconds);
};
