// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "NavGrid.h"
#include "GridPawn.h"
#include "Tiles/GridTile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GridMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

UGridMovementComponent::UGridMovementComponent(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{	
	Spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this,"Path");
}

void UGridMovementComponent::BeginPlay()
{	
	TActorIterator<ANavGrid> Itr(GetWorld());
	if (Itr)
	{
		Grid = *Itr;
	}
}

void UGridMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Moving)
	{		
		float CurrentSpeed = MaxWalkingSpeed*DeltaTime;				
		Distance = FMath::Min(Spline->GetSplineLength(), Distance + CurrentSpeed);
		AActor *Owner = GetOwner();
		FTransform OldTransform=Owner->GetActorTransform();

		FTransform NewTransform = Spline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		FRotator DesiredRotation=NewTransform.Rotator();
		//Owner->SetActorRotation(DesiredRotation);
		Owner->SetActorTransform(NewTransform);
		if (Distance >= Spline->GetSplineLength())
		{			
			Moving = false;
			Distance = 0;
			Velocity = FVector::ZeroVector;
		}
		else
		{
			Velocity = (NewTransform.GetLocation() - OldTransform.GetLocation())*(1 / DeltaTime);
		}
		UpdateComponentVelocity();
	}
}

UGridTileComponent* UGridMovementComponent::GetCurrentTile()
{
	return CurrentTile;
}

void UGridMovementComponent::SetCurrentTile(UGridTileComponent *Tile)
{
	CurrentTile = Tile;
}

bool UGridMovementComponent::CreatePath(UGridTileComponent &Target)
{	
	CurrentPath.Empty();
	Spline->ClearSplinePoints();
	AGridPawn *Pawn = Cast<AGridPawn>(GetOwner());
	CurrentTile = Grid->GetTile(Pawn->GetActorLocation());	
	if (CurrentTile != NULL )
	{
		TArray<UGridTileComponent*> Range;
		Grid->TilesInRange(CurrentTile, Range, Pawn, true);		
		if (Range.Contains(&Target))
		{			
			UGridTileComponent *Current = &Target;
			while (Current)
			{				
				CurrentPath.Add(Current);
				Current = Current->Backpointer;
			}		
			Algo::Reverse(CurrentPath);		

			Spline->AddSplinePoint(Pawn->GetActorLocation(), ESplineCoordinateSpace::Local);
			Spline->SetSplinePointType(0, ESplinePointType::Linear);
			for (int i = 1; i < CurrentPath.Num(); ++i)
			{
				CurrentPath[i]->AddSplinePoint(*Spline);
				Spline->SetSplinePointType(i, ESplinePointType::Linear);
				print(FString::FromInt(Spline->GetNumSplinePoints()));
			}			
			return true;
		}
	}
	else
	{
		print("Create Path CurrentTile NULL");
	}
	print("false");
	return false;
}

void UGridMovementComponent::ShowPath()
{
	print(FString::FromInt(CurrentPath.Num()));
	if (CurrentPath.Num() >= 1)
	for (int i = 1; i < CurrentPath.Num();++i)
	{
		AActor *Actor=CurrentPath[i]->GetOwner();
		AGridTile *TileActor = Cast<AGridTile>(Actor);
		if (IsValid(TileActor))
		{
			TileActor->EnablePathOverlay();
		}		
	}
}

void UGridMovementComponent::HidePath()
{
	if (CurrentPath.Num() != 0)
	{
		for (UGridTileComponent *T : CurrentPath)
		{
			AActor *Actor = T->GetOwner();
			AGridTile *TileActor = Cast<AGridTile>(Actor);
			if (IsValid(TileActor))
			{
				TileActor->DisablePathOverlay();
			}
		}
	}
}

bool UGridMovementComponent::MoveTo(UGridTileComponent &Target)
{
	if (CreatePath(Target))
	{
		FollowPath();
		return true;
	}
	return false;
}

void UGridMovementComponent::FollowPath()
{
	Moving = true;
}

void UGridMovementComponent::StopMoving()
{
	Moving = false;
}