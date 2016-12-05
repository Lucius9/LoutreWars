// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridTile.h"



// Sets default values
AGridTile::AGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	SpriteComponent->SetSprite(NULL);
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetCollisionProfileName("OverlapAllDynamic");

	FocusOverlaySpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FocusOverlaySprite"));
	FocusOverlaySpriteComponent->SetupAttachment(RootComponent);
	FocusOverlaySpriteComponent->SetSprite(NULL);
	FocusOverlaySpriteComponent->SetRelativeLocation(FVector(0.0f, 0.1f, 0.0f));
	FocusOverlaySpriteComponent->SetCollisionProfileName("OverlapAllDynamic");
	DisableCurrentTileOverlay();

	BottomOverlaySpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BottomOverlaySprite"));
	BottomOverlaySpriteComponent->SetupAttachment(RootComponent);
	BottomOverlaySpriteComponent->SetSprite(NULL);
	BottomOverlaySpriteComponent->SetRelativeLocation(FVector(0.0f, 0.2f, 0.0f));
	BottomOverlaySpriteComponent->SetCollisionProfileName("OverlapAllDynamic");	
	DisableMovementRangeOverlay();

	TopOverlaySpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("TopOverlaySprite");
	TopOverlaySpriteComponent->SetupAttachment(RootComponent);
	TopOverlaySpriteComponent->SetSprite(NULL);
	TopOverlaySpriteComponent->SetRelativeLocation(FVector(0.0f, 0.3f, 0.0f));
	TopOverlaySpriteComponent->SetCollisionProfileName("OverlapAllDynamic");
	DisablePathOverlay();

	GridTileComponent = CreateDefaultSubobject<UGridTileComponent>(TEXT("GridTileComponent"));
	GridTileComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AGridTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AGridTile::EnableMovementRangeOverlay()
{
	BottomOverlaySpriteComponent->SetSprite(MovementRangeOverlaySprite);
	BottomOverlaySpriteComponent->SetVisibility(true, true);
	BottomOverlaySpriteComponent->SetHiddenInGame(false);	
}

void AGridTile::DisableMovementRangeOverlay()
{
	BottomOverlaySpriteComponent->SetSprite(NULL);
	BottomOverlaySpriteComponent->SetVisibility(false, true);
	BottomOverlaySpriteComponent->SetHiddenInGame(true);
}

void AGridTile::EnablePathOverlay()
{
	TopOverlaySpriteComponent->SetSprite(PathOverlaySprite);
	TopOverlaySpriteComponent->SetVisibility(true, true);
	TopOverlaySpriteComponent->SetHiddenInGame(false);
}

void AGridTile::DisablePathOverlay()
{
	TopOverlaySpriteComponent->SetSprite(NULL);
	TopOverlaySpriteComponent->SetVisibility(false, true);
	TopOverlaySpriteComponent->SetHiddenInGame(true);
}

void AGridTile::EnableAttackRangeOverlay()
{
	BottomOverlaySpriteComponent->SetSprite(AttackRangeOverlaySprite);
	BottomOverlaySpriteComponent->SetVisibility(true, true);
	BottomOverlaySpriteComponent->SetHiddenInGame(false);
}

void AGridTile::DisableAttackRangeOverlay()
{
	BottomOverlaySpriteComponent->SetSprite(NULL);
	BottomOverlaySpriteComponent->SetVisibility(false, true);
	BottomOverlaySpriteComponent->SetHiddenInGame(true);
}

void AGridTile::EnableAttackOverlay()
{
	BottomOverlaySpriteComponent->SetSprite(AttackOverlaySprite);
	BottomOverlaySpriteComponent->SetVisibility(true, true);
	BottomOverlaySpriteComponent->SetHiddenInGame(false);
}

void AGridTile::DisableAttackOverlay()
{
	BottomOverlaySpriteComponent->SetSprite(NULL);
	BottomOverlaySpriteComponent->SetVisibility(false, true);
	BottomOverlaySpriteComponent->SetHiddenInGame(true);
}

void AGridTile::EnableCurrentTileOverlay()
{
	FocusOverlaySpriteComponent->SetSprite(CurrentTileOverlaySprite);
	FocusOverlaySpriteComponent->SetVisibility(true, true);
	FocusOverlaySpriteComponent->SetHiddenInGame(false);
}

void AGridTile::DisableCurrentTileOverlay()
{
	FocusOverlaySpriteComponent->SetSprite(NULL);
	FocusOverlaySpriteComponent->SetVisibility(false, true);
	FocusOverlaySpriteComponent->SetHiddenInGame(true);
}
