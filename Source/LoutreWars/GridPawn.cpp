// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "NavGrid.h"
#include "GridPawnPlayerController.h"
#include "GridPlayerState.h"
#include "LoutreWarsGameMode.h"
#include "GridPawn.h"

//#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
AGridPawn::AGridPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent *Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(RootComponent);	
	Capsule->SetCapsuleRadius(9.0f);
	Capsule->SetCapsuleHalfHeight(9.0f);
	Capsule->SetRelativeLocation(FVector(0, 10.0f, 0));
	//Capsule->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);		
	
	
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("SpriteComponent");
	SpriteComponent->SetupAttachment(RootComponent);

	MovementComponent=CreateDefaultSubobject<UGridMovementComponent>("MovementComponent");	

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);
	Arrow->ArrowSize = 0.1f;
	Arrow->SetHiddenInGame(false,true);

	OnClicked.AddUniqueDynamic(this, &AGridPawn::OnActorClick);
	OnInputTouchEnd.AddDynamic(this, &AGridPawn::OnActorTouched);

	this->AIControllerClass = NULL;
	this->AutoPossessAI = EAutoPossessAI::Disabled;
	this->AutoPossessPlayer = EAutoReceiveInput::Disabled;
		
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	ANavGrid* Grid = ANavGrid::GetNavGrid(GetWorld());
	if (Grid!=NULL)
	{		
		UGridTileComponent *CurrentTile = Grid->GetTile(GetActorLocation());
		if (CurrentTile != NULL)
		{			
			SetActorLocation(CurrentTile->GetPawnLocation());			
		}
		else
		{
			//print("BeginPlay CurrentTile NULL");
		}
	}
	else
	{
		//print("Begin Play Grid NULL");
	}
	
}

// Called every frame
void AGridPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGridPawn::OnActorTouched(ETouchIndex::Type FingerIndex, AActor *Actor)
{
	ANavGrid *Grid = ANavGrid::GetNavGrid(GetWorld());
	if (Grid)
	{
		UGridTileComponent *CurrentTile = Grid->GetTile(GetActorLocation());
		if (CurrentTile)
		{
			Grid->OnTileTouched().Broadcast(*CurrentTile);
		}
	}
}

void AGridPawn::OnActorClick(AActor *Actor, FKey Key)
{
	ANavGrid* Grid = ANavGrid::GetNavGrid(GetWorld());
	if (Grid)
	{
		UGridTileComponent *CurrentTile = Grid->GetTile(GetActorLocation());
		if (CurrentTile)
		{
			Grid->OnTileClicked().Broadcast(*CurrentTile);
		}
	}
	/*APlayerController *SomeController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AGridPawnPlayerController *GPPC = Cast<AGridPawnPlayerController>(SomeController);

	if (GPPC && GPPC->CanSelectPawn(this))
	{
		GPPC->SelectPawn(this);
		if (!MovementComponent->IsMoving())
		{
			if (MovementComponent->HasMoved == false && !HasPlayed)
			{
				GPPC->EnableMovementWidget();
			}
			else if (!HasPlayed)
			{
				GPPC->EnableEndMovementWidget();
			}
		}
	}	*/
}

bool AGridPawn::IsMoving()
{
	return MovementComponent->IsMoving();
}

bool AGridPawn::IsAttackableBy(AGridPawn *Attacker) 
{
	if (this->Faction != Attacker->Faction && this->Faction->TeamIndex != Attacker->Faction->TeamIndex)
	{
		return true;
	}
	return false;
}

void AGridPawn::Attack(AGridPawn *Defenser, bool attackable)
{
	int tmp;
	if (attackable)
	{
		tmp = (Atc * Unit) - (Defenser->Def * Defenser->Unit);
		Defenser->PV = Defenser->PV - tmp;
		if (Defenser->PV) {
			Defenser->Unit = Defenser->PV / Defenser->UnitPv;
			tmp = (Defenser->Atc * Defenser->Unit) - (Def * Unit);
			PV = PV - tmp;
			if (PV)
			{
				Unit = PV / UnitPv;
			}	
			//print("Attack");
		}
	}
}

void AGridPawn::BeginTurn()
{
	HasMoved = false;
	HasPlayed = false;	
}

/*void AGridPawn::OnCapsuleClick(UPrimitiveComponent* pComponent, FKey inKey)
{
	print("lol");
}*/

/*AActor* AGridPawn::OnActorTouch(ETouchIndex::Type FingerIndex, AActor *Actor)
{
	print("here");
	return NULL;
}*/

