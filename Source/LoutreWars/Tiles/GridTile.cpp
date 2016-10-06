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

	OverlaySpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("OverlaySprite"));
	OverlaySpriteComponent->SetupAttachment(RootComponent);
	OverlaySpriteComponent->SetSprite(NULL);
	OverlaySpriteComponent->SetRelativeLocation(FVector(0.0f, 0.1f, 0.0f));
	OverlaySpriteComponent->SetCollisionProfileName("OverlapAllDynamic");	
	DisableRangeOverlay();

	PathOverlaySpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("PathOverlaySprite");
	PathOverlaySpriteComponent->SetupAttachment(RootComponent);
	PathOverlaySpriteComponent->SetSprite(NULL);
	PathOverlaySpriteComponent->SetRelativeLocation(FVector(0.0f, 0.2f, 0.0f));
	PathOverlaySpriteComponent->SetCollisionProfileName("OverlapAllDynamic");
	DisablePathOverlay();

	GridTileComponent = CreateDefaultSubobject<UGridTileComponent>(TEXT("GridTileComponent"));
	GridTileComponent->SetupAttachment(RootComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetCollisionProfileName("OverlapAllDynamic");
	FlipbookComponent->SetupAttachment(RootComponent);


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

void AGridTile::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	/*FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGridTile, Sprite))
	{
		SpriteComponent->SetSprite(Sprite);
		Flipbook = NULL;
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AGridTile, Flipbook))
	{
		FlipbookComponent->SetFlipbook(Flipbook);
		Sprite = NULL;
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGridTile, Cost))
	{
		GridTileComponent->SetCost(Cost);
	}*/
}

void AGridTile::EnableRangeOverlay()
{
	//OverlaySpriteComponent->SetVisibility(true, true);
	OverlaySpriteComponent->SetHiddenInGame(false);	
}

void AGridTile::DisableRangeOverlay()
{
	//OverlaySpriteComponent->SetVisibility(false, true);
	OverlaySpriteComponent->SetHiddenInGame(true);
}

void AGridTile::EnablePathOverlay()
{
	//PathOverlaySpriteComponent->SetVisibility(true, true);
	PathOverlaySpriteComponent->SetHiddenInGame(false);
}

void AGridTile::DisablePathOverlay()
{
	//PathOverlaySpriteComponent->SetVisibility(false, true);
	PathOverlaySpriteComponent->SetHiddenInGame(true);
}
