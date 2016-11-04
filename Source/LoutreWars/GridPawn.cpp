// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "NavGrid.h"
#include "GridPawnPlayerController.h"
#include "GridPlayerState.h"
#include "GridPawn.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

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
	//OnInputTouchEnd.AddDynamic(this, &AGridPawn::OnActorTouch);	
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	ANavGrid* Grid = ANavGrid::GetNavGrid(GetWorld());
	if (Grid!=NULL)
	{
		print(GetActorLocation().ToString());
		UGridTileComponent *CurrentTile = Grid->GetTile(GetActorLocation());
		if (CurrentTile != NULL)
		{			
			SetActorLocation(CurrentTile->GetPawnLocation());			
		}
		else
		{
			print("BeginPlay CurrentTile NULL");
		}
	}
	else
	{
		print("Begin Play Grid NULL");
	}
	
}

// Called every frame
void AGridPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGridPawn::OnActorClick(AActor *Actor, FKey Key)
{		
	if (!MovementComponent->IsMoving())
	{
		APlayerController *SomeController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AGridPawnPlayerController *GPPC = Cast<AGridPawnPlayerController>(SomeController);
		if (GPPC && GPPC->GetPawn() != this)
		{
			GPPC->Possess(this);
			GPPC->EnableMovementMode();
		}
		TActorIterator<ANavGrid>Itr(GetWorld());
		if (*Itr != NULL)
		{
			Itr->HideTilesInRange();
			UGridTileComponent* CurrentTile = Itr->GetTile(GetActorLocation());
			print(CurrentTile->GetOwner()->GetActorLabel());
			if (CurrentTile)
			{
				CurrentTile->UnderCurrentPawn = true;
				TArray<UGridTileComponent*> Out;
				Itr->ShowTilesInRange(CurrentTile, this);
			}
			else
			{
				print("CurrentTile NULL");
			}
		}
		else
		{
			print("Itr null");
		}
	}	
}

bool AGridPawn::IsMoving()
{
	return MovementComponent->IsMoving();
}

bool AGridPawn::IsAttackableBy(AGridPawn *Attacker) 
{
	if (Attacker->ControlledByAI == ControlledByAI && Attacker->PlayerIndex == PlayerIndex)
	{
		return false;
	}
	AGridPlayerState *MyState = Cast<AGridPlayerState>(PlayerState);
	AGridPlayerState *AttackerState = Cast<AGridPlayerState>(Attacker->PlayerState);
	if (MyState && AttackerState)
	{
		if (MyState->TeamIndex == AttackerState->TeamIndex)
		{
			return false;
		}
		return true;
	}
	else
	{
		print("cast failed");
		return false;
	}
	
}

void AGridPawn::Attack(AGridPawn *Attacker, bool attackable)
{
	int tmp;
	if (attackable)
	{
		tmp = (Attacker->Atc * Attacker->Unit) - (Def * Unit);
		PV = PV - tmp;
		if (PV)
			Unit = PV / UnitPv;
	}
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

