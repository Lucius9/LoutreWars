// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "Components/GridTileComponent.h"
#include "Components/SplineComponent.h"
#include "GridMovementComponent.generated.h"


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

	/** Desired pitch rotation after current movement is complete */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TargetPitch;

	/** The current pitch rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CurrentPitch;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementMode")
	TArray<EGridMovementMode> AvailableMovementModes;

public :
	static float FaceUp;
	static float FaceDown;
	static float FaceLeft;
	static float FaceRight;	

	bool HasMoved = false;

protected:	
	UGridTileComponent *CurrentTile;
	TArray<UGridTileComponent *>CurrentPath;
	class ANavGrid *Grid;
	bool Moving=false;	
	float Distance = 0;
	TArray<UGridTileComponent *>OpenList;
	TArray<UGridTileComponent*> ClosedList;


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
	bool CreatePath2(UGridTileComponent *Target);
	void ShowPath();
	void HidePath();
	bool MoveTo(UGridTileComponent &Target);
	void FollowPath();
	void StopMoving();
	UFUNCTION(BlueprintCallable,Category="Movement")
	void EndMovement();
	UFUNCTION(BlueprintCallable, Category="Movement")
	void GoBackOnPreviousTile();

protected :	
	void InsertInOpenList(UGridTileComponent* Tile);
	bool ComputePath(UGridTileComponent *From, UGridTileComponent *To);
	float ComputeTargetPitch(const FVector &PawnLocation, const FVector &Target);
	float ComputeDeltaPitch(const float &OldPitch, const float &NewPitch);
	float LimitRotation(const float &OldPitch, const float &NewPitch, float DeltaSeconds);

public:
	DECLARE_EVENT_OneParam (UGridMovementComponent,FOnMovementDone,AGridPawn&)
	FOnMovementDone& OnMovementEnd() { return OnMovementEndEvent; }

private:
	FOnMovementDone OnMovementEndEvent;
};
