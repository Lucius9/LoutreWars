// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "limits"
#include "NavGrid.h"
#include "GridTileComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values for this component's properties
UGridTileComponent::UGridTileComponent(const FObjectInitializer &ObjectInitializer) 
	:Super(ObjectInitializer)
{
	Extent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,"Extent");
	Extent->SetupAttachment(this);
	Extent->ShapeColor = FColor::White;
	Extent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Extent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Extent->SetCollisionResponseToChannel(ANavGrid::ECC_Tile, ECollisionResponse::ECR_Block);
	Extent->SetHiddenInGame(false);
	Extent->OnBeginCursorOver.AddDynamic(this, &UGridTileComponent::OnTileCursorOver);	


	PawnLocation = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"PawnLocation");
	PawnLocation->SetupAttachment(this);
	PawnLocation->SetRelativeLocation(FVector(0.0f, 1.0f, 0.0f));
	

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
		UPaperSpriteComponent *MainSpriteComponent = Cast<UPaperSpriteComponent>(Child);
		if (FlipbookComponent)
		{
			if (FlipbookComponent->GetFlipbook() != NULL)
			{
				UPaperSprite *Sprite = FlipbookComponent->GetFlipbook()->GetSpriteAtFrame(0);
				Size = Sprite->GetSourceSize();
				sizeInitialized = true;

			}
		}
		else if (MainSpriteComponent)
		{
			if (MainSpriteComponent->GetSprite() != NULL)
			{
				Size = MainSpriteComponent->GetSprite()->GetSourceSize();
				sizeInitialized = true;

			}			
		}
	}
	if (!sizeInitialized)
	{
		Size = FVector2D(16.0f, 16.0f);
	}
	Extent->SetBoxExtent(FVector((Size.X)/2,0.0f,(Size.Y)/2));
}

void UGridTileComponent::BeginPlay()
{
	if (!Grid)
	{
		Grid = ANavGrid::GetNavGrid(GetWorld());
	}	
}

float UGridTileComponent::GetCost()
{
	return Cost;
}

void UGridTileComponent::SetCost(float cost)
{
	this->Cost = cost;
}

void UGridTileComponent::ComputeNeighbours()
{	
	UWorld *World = this->GetWorld();
	for (TObjectIterator<UGridTileComponent> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() == World && *Itr != this)
		{			
			float distance = (GetComponentLocation() - Itr->GetComponentLocation()).Size();			
			
			//We don't naviate in diagonal, so the max distance between neighbours should be the greatest box extent coordinate multiplicated by 2
			float MaxLenght = FMath::Max(Extent->GetScaledBoxExtent().X, Extent->GetScaledBoxExtent().Z);				
			if (distance <= MaxLenght*2)
			{
				Neighbours.Add(*Itr);					
			}		
		}
	}
}


TArray<UGridTileComponent*>* UGridTileComponent::GetNeighbours()
{
	if (Neighbours.Num()==0)
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
	FVector Start = FromPos + CollisionCapsule.RelativeLocation;	
	FVector End = To +  CollisionCapsule.RelativeLocation;	
	FQuat Rot = FQuat::Identity;
	UWorld * World = CollisionCapsule.GetWorld();

	FCollisionShape CollisionShape = CollisionCapsule.GetCollisionShape();
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(CollisionCapsule.GetOwner());
	CollisionQueryParam.AddIgnoredComponent(&CollisionCapsule);
	FCollisionResponseParams CollisionResponseParam;
	bool Hit =World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionQueryParam, CollisionResponseParam);	
	/*if (Hit)
	{
		if (OutHit.Actor!=NULL)print(OutHit.Actor->GetActorLabel());
		if (OutHit.Component != NULL)print(OutHit.Component->GetOwner()->GetActorLabel());
		DrawDebugLine(World, OutHit.TraceStart, OutHit.TraceEnd, FColor::Red, true);
	}
	else
	{
		DrawDebugLine(World, OutHit.TraceStart, OutHit.TraceEnd, FColor::Blue, true);
	}*/
		
	return Hit;
}

void UGridTileComponent::GetUnobstructedNeighbours(const UCapsuleComponent & CollisionCapsule, TArray<UGridTileComponent*>& OutNeighbours)
{
	OutNeighbours.Empty();
	TArray<UGridTileComponent*> *TileNeighbours = GetNeighbours();	
	for (UGridTileComponent *N : *TileNeighbours)
	{
		if (!N->Obstructed(PawnLocation->GetComponentLocation(), CollisionCapsule))
		{
			OutNeighbours.Add(N);
		}		
	}
}

void UGridTileComponent::ResetPath()
{
	Backpointer = NULL;
	Distance = std::numeric_limits<float>::infinity();
	Visited = false;
}

FVector UGridTileComponent::GetPawnLocation()
{
	return PawnLocation->GetComponentLocation();
}

void UGridTileComponent::OnTileCursorOver(UPrimitiveComponent* TouchedComponent)
{
	if (Grid && !UnderCurrentPawn)
	{
		Grid->TileCursorOver(*this);
	}	
}