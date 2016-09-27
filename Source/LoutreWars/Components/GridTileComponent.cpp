// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridTileComponent.h"


// Sets default values for this component's properties
UGridTileComponent::UGridTileComponent(const FObjectInitializer &ObjectInitializer) 
	:Super(ObjectInitializer)
{
	Extent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,"Extent");
	Extent->SetupAttachment(this);
	Extent->ShapeColor = FColor::White;
	

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGridTileComponent::OnComponentCreated()
{
	Super::OnComponentCreated();	
	TArray<USceneComponent*>Children;
	this->GetAttachParent()->GetChildrenComponents(false, Children);
	FVector2D Size;
	bool sizeInitialized = false;
	for (int i = 0; i < Children.Num();++i)
	{
		USceneComponent* Child = Children[i];
		UPaperFlipbookComponent *FlipbookComponent = Cast<UPaperFlipbookComponent>(Child);
		UPaperSpriteComponent *SpriteComponent = Cast<UPaperSpriteComponent>(Child);
		if (FlipbookComponent)
		{
			if (FlipbookComponent->GetFlipbook() != NULL)
			{
				UPaperSprite *Sprite = FlipbookComponent->GetFlipbook()->GetSpriteAtFrame(0);
				Size = Sprite->GetSourceSize();
				sizeInitialized = true;

			}
		}
		else if (SpriteComponent)
		{
			if (SpriteComponent->GetSprite() != NULL)
			{
				Size = SpriteComponent->GetSprite()->GetSourceSize();
				sizeInitialized = true;

			}			
		}
	}
	if (!sizeInitialized)
	{
		Size = FVector2D(16.0f, 16.0f);
	}
	Extent->SetBoxExtent(FVector((Size.X)/2,2.0f,(Size.Y)/2));
}

float UGridTileComponent::GetCost()
{
	return Cost;
}

void UGridTileComponent::SetCost(float cost)
{
	this->Cost = cost;
}

void UGridTileComponent::ComputeContactPoints()
{
	int XExtent = Extent->GetScaledBoxExtent().X;
	int ZExtent = Extent->GetScaledBoxExtent().Z;
	for (int i = -XExtent; i < XExtent; i += XExtent)
	{
		for (int j = -ZExtent; j < ZExtent; j += ZExtent)
		{
			FVector ComponentLocation = FVector(i, 0, j);
			FVector WorldLocation = GetComponentLocation() + ComponentLocation;
			ContactPoints.Add(WorldLocation);
		}
	}
}

void UGridTileComponent::ComputeNeighbours()
{
	UWorld *World = this->GetWorld();
	for (TObjectIterator<UGridTileComponent> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() == World && *Itr != this)
		{
			bool Added = false; // stop comparing CPs when we know a tile is a neighbour
			for (const FVector &OtherCP : *Itr->GetContactPoints())
			{
				for (const FVector &MyCP : *GetContactPoints())
				{
					// the max length between 2 neightbours is the maximum BoxExtent size on X or Z cause we don't move in diagonal
					int MaxLenght = FMath::Max(Extent->GetScaledBoxExtent().X, Extent->GetScaledBoxExtent().Z);

					if ((OtherCP - MyCP).Size() <= MaxLenght)
					{
						Neighbours.Add(*Itr);
						Added = true;
						break;
					}
				}
				if (Added) { break; }
			}
		}
	}
}

TArray<FVector>* UGridTileComponent::GetContactPoints()
{
	if (!ContactPoints.Num())
	{
		ComputeContactPoints();
	}
	return &ContactPoints;
}

TArray<UGridTileComponent*>* UGridTileComponent::GetNeighbours()
{
	if (!Neighbours.Num())
	{
		ComputeNeighbours();
	}
	return &Neighbours;
}
/*
TODO add movementmode for legacy 
	example: water tile accessible only for swimming pawns
*/
bool UGridTileComponent::Traversable(float MaxWalkAngle)
{
	FRotator Rot = GetComponentRotation();
	if (Rot.Pitch < MaxWalkAngle && Rot.Pitch > -MaxWalkAngle)
	{
		return true;
	}
	return false;
}

bool UGridTileComponent::Obstructed(const FVector & FromPos, const UCapsuleComponent & CollisionCapsule)
{
	return Obstructed(FromPos, GetComponentLocation(), CollisionCapsule);
}

bool UGridTileComponent::Obstructed(const FVector &FromPos, const FVector & To, const UCapsuleComponent & CollisionCapsule)
{
	FHitResult OutHit;
	FVector Start = Start + CollisionCapsule.RelativeLocation;
	FVector End = Start + CollisionCapsule.RelativeLocation;
	FQuat Rot = FQuat::Identity;
	UWorld * World = CollisionCapsule.GetWorld();

	FCollisionShape CollisionShape = CollisionCapsule.GetCollisionShape();
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(CollisionCapsule.GetOwner());
	FCollisionResponseParams CollisionResponseParam;

	bool Hit= World->SweepSingleByChannel(OutHit, Start, End, Rot, ECollisionChannel::ECC_Pawn, CollisionShape, CollisionQueryParam, CollisionResponseParam);
	

	return Hit;
}

void UGridTileComponent::GetUnobstructedNeighbours(const UCapsuleComponent & CollisionCapsule, TArray<UGridTileComponent*>& OutNeighbours)
{
	OutNeighbours.Empty();
	for (UGridTileComponent *N : *GetNeighbours())
	{
		if (!N->Obstructed(GetComponentLocation(), CollisionCapsule))
		{
			OutNeighbours.Add(N);
		}
	}
}

