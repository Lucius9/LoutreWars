// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "NavGrid.h"
#include "GridPawn.h"
#include "Tiles/GridTile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GridMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

float UGridMovementComponent::FaceRight = 0.0;
float UGridMovementComponent::FaceLeft = 180.0f;
float UGridMovementComponent::FaceUp = -90.0f;
float UGridMovementComponent::FaceDown = 90.0f;

UGridMovementComponent::UGridMovementComponent(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{	
	Spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, "SplineComponent");

	CurrentPitch = FaceRight;
	TargetPitch = FaceRight;
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
		FVector OldLocation = Owner->GetActorLocation();			
		FTransform NewTransform = Spline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);		
		FVector NewLocation = NewTransform.GetLocation();			

		TargetPitch = ComputeTargetPitch(OldLocation,NewLocation);		
		CurrentPitch = LimitRotation(CurrentPitch, TargetPitch, DeltaTime);

		FQuat CurrentQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(CurrentPitch));		

		Owner->SetActorLocationAndRotation(NewLocation, CurrentQuat);
	
		if (Distance >= Spline->GetSplineLength())
		{			
			Moving = false;
			Distance = 0;
			Velocity = FVector::ZeroVector;
		}
		else
		{			
			Velocity = (NewLocation - OldLocation)*(1 / DeltaTime);
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

bool UGridMovementComponent::IsMoving()
{
	return Moving;
}

bool UGridMovementComponent::CreatePath(UGridTileComponent &Target)
{	
	CurrentPath.Empty();
	Spline->ClearSplinePoints(true);		
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

			print("Actor Location " + Pawn->GetActorLocation().ToString());
			Spline->AddSplinePoint(Pawn->GetActorLocation(), ESplineCoordinateSpace::Local);
			Spline->SetSplinePointType(0, ESplinePointType::Linear);
			for (int i = 1; i < CurrentPath.Num(); ++i)
			{
			
				CurrentPath[i]->AddSplinePoint(*Spline);				
				Spline->SetSplinePointType(i, ESplinePointType::Linear);
				
			}				
			return true;
		}
	}		
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

float UGridMovementComponent::LimitRotation(const float &OldPitch, const float &NewPitch, float DeltaSeconds)
{		
	if (FMath::IsNearlyEqual(OldPitch, NewPitch))
	{		
		return NewPitch;
	}

	float DeltaDegrees = ComputeDeltaPitch(OldPitch, NewPitch);

	DeltaDegrees = FMath::Clamp(DeltaDegrees, MaxRotationSpeed * -DeltaSeconds, MaxRotationSpeed * DeltaSeconds);	
	
	return OldPitch+DeltaDegrees;	
}

float UGridMovementComponent::ComputeDeltaPitch(const float &OldPitch, const float &NewPitch)
{
	float DeltaPitch = NewPitch - OldPitch;
	if (FMath::Abs(DeltaPitch) <= 180.0f)
	{
		return DeltaPitch;
	}
	else
	{
		if (DeltaPitch > 0)
		{			
			DeltaPitch -= 360;			
		}
		else
		{			
			DeltaPitch += 360;
		}
		return DeltaPitch;
	}
}

float UGridMovementComponent::ComputeTargetPitch(const FVector &PawnLocation, const FVector &Target)
{
	FVector Delta = Target-PawnLocation;	

	if (Delta.Z > 0.0f)
	{			
		return FaceUp;
	}
	if (Delta.Z < 0.0f)
	{				
		return FaceDown;
	}
	if (Delta.X < 0.0f)
	{
		return FaceLeft;
	}
	return FaceRight;
}