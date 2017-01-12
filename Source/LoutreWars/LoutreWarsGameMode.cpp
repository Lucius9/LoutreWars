// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridPawn.h"
#include "GridPlayerState.h"
#include "Levels/LWWorldSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "LoutreWarsGameMode.h"

//#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

ALoutreWarsGameMode::ALoutreWarsGameMode()
{
	//DefaultPawnClass = AGridPawn::StaticClass();
	PlayerControllerClass = AGridPawnPlayerController::StaticClass();

	PlayerStateClass = AGridPlayerState::StaticClass();
}

void ALoutreWarsGameMode::BeginPlay()
{
	Super::BeginPlay();

	ALWWorldSettings *WorldSettings = Cast<ALWWorldSettings>(GetWorldSettings());	
	AGridPawnPlayerController *Player0=Cast<AGridPawnPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (Player0)
	{	
		for (TActorIterator<AFaction> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->Controller == NULL)
			{
				if (UKismetMathLibrary::ClassIsChildOf(ActorItr->ControllerClass, AAIController::StaticClass()))
				{
					InitAI(*ActorItr);
				}
				else if (UKismetMathLibrary::ClassIsChildOf(ActorItr->ControllerClass, AGridPawnPlayerController::StaticClass()))
				{
					InitPlayer(*ActorItr);
				}
			}
		}
	}
	InitPlayerOrder();
	EnableNavigationWidget();
}

void ALoutreWarsGameMode::InitPlayer(AFaction *Faction)
{
	UWorld *World = GetWorld();	
	AController *Controller = UGameplayStatics::GetPlayerController(World, Faction->PlayerNumber);
	AGridPawnPlayerController *PlayerController=Cast<AGridPawnPlayerController>(Controller);
	if (PlayerController)
	{			
		PlayerController->Faction = Faction;
		PlayerController->InitPawns();
		Faction->Controller = PlayerController;				
	}
	else
	{
		PlayerController=Cast<AGridPawnPlayerController>(World->SpawnActor(Faction->ControllerClass));
		if (PlayerController)
		{							
			PlayerController->Faction = Faction;
			PlayerController->InitPawns();
			Faction->Controller = PlayerController;		
			UGameInstance *GameInstance=World->GetGameInstance();
			FString OutError;
			ULocalPlayer *NewPlayer=GameInstance->CreateLocalPlayer(PlayerController->GetUniqueID(),OutError,false);
			if (NewPlayer)
			{
				PlayerController->SetPlayer(NewPlayer);				
			}
			else
			{
				//print(OutError);
			}			
		}
	}
}

void ALoutreWarsGameMode::InitAI(AFaction *Faction)
{	
	UWorld *World = GetWorld();
	AGridAIController *Controller=Cast<AGridAIController>(World->SpawnActor(Faction->ControllerClass));
	if (Controller)
	{
		Faction->Controller = Controller;
		Controller->Faction = Faction;		
		//Controller->OnAITurnEnd().AddUObject(this, &ALoutreWarsGameMode::OnPlayersTurnEnd);
	}
}

void ALoutreWarsGameMode::InitPlayerOrder()
{
	
	TArray<AGridPawnPlayerController*> HumanPlayers;
	for (TActorIterator<AGridPawnPlayerController>PCItr(GetWorld()) ; PCItr; ++PCItr)
	{
		HumanPlayers.Add(*PCItr);
	}
	int StartingPlayer = UKismetMathLibrary::RandomIntegerInRange(0, (HumanPlayers.Num() - 1));
	HumanPlayers[StartingPlayer]->CurrentPlayerController = true;
	PlayersOrder.Add(HumanPlayers[StartingPlayer]);
	for (int i = 0; i < StartingPlayer; ++i)
	{
		PlayersOrder.Add(HumanPlayers[i]);
	}
	for (int i = StartingPlayer + 1; i < HumanPlayers.Num(); ++i)
	{
		PlayersOrder.Add(HumanPlayers[i]);
	}
	
	
	for (TActorIterator<AGridAIController>AIItr(GetWorld()); AIItr; ++AIItr)
	{
		PlayersOrder.Add(*AIItr);
	}
	CurrentPlayerIndex = 0;
}

void ALoutreWarsGameMode::EnableMovementWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->EnableMovementWidget();
	}
}

void ALoutreWarsGameMode::EnableEndMovementWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->EnableEndMovementWidget();
	}
}

void ALoutreWarsGameMode::EnableAttackWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->EnableAttackWidget();
	}
}

void ALoutreWarsGameMode::DisableActionWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->DisableActionWidget();
	}
}

void ALoutreWarsGameMode::EnableNavigationWidget()
{
	if (NavigationWidget == nullptr)
	{
		APlayerController *Controller = Cast<APlayerController>(PlayersOrder[CurrentPlayerIndex]);		
		NavigationWidget = CreateWidget<UNavigationWidget>(Controller, NavigationWidgetClass);
		NavigationWidget->AddToViewport();
	}
}
void ALoutreWarsGameMode::DisableNavigationWidget()
{
	if (NavigationWidget != nullptr)
	{		
		NavigationWidget->RemoveFromViewport();
		NavigationWidget = nullptr;
	}
}

void ALoutreWarsGameMode::UpdateTileWidget()
{
	if (NavigationWidget != nullptr)
	{
		//NavigationWidget->HideTileWidget();
		NavigationWidget->UpdateTileWidget();
	}
}

void ALoutreWarsGameMode::HideTileWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->HideTileWidget();
	}
}

void ALoutreWarsGameMode::UpdatePawnWidget()
{
	if (NavigationWidget != nullptr)
	{		
		NavigationWidget->UpdatePawnWidget();
	}

}
void ALoutreWarsGameMode::HidePawnWidget()
{
	if (NavigationWidget != nullptr)
	{
		NavigationWidget->HidePawnWidget();
	}
}

void ALoutreWarsGameMode::SwitchCurrentPlayer()
{
	if (CurrentPlayerIndex == PlayersOrder.Num() - 1)
	{
		CurrentPlayerIndex = 0;
		++TurnNumber;
	}
	else
	{
		++CurrentPlayerIndex;
	}
	if (PlayersOrder[CurrentPlayerIndex]->GetClass()->IsChildOf(AGridPawnPlayerController::StaticClass()))
	{
		EnableNavigationWidget();
		AGridPawnPlayerController *PlayerController = Cast<AGridPawnPlayerController>(PlayersOrder[CurrentPlayerIndex]);
		if (PlayerController)
		{
			PlayerController->CurrentPlayerController = true;
			PlayerController->BeginTurn();
		}
	}
	else if(PlayersOrder[CurrentPlayerIndex]->GetClass()->IsChildOf(AGridAIController::StaticClass()))
	{
		AGridAIController *AIController = Cast<AGridAIController>(PlayersOrder[CurrentPlayerIndex]);
		if (AIController)
		{
			AIController->BeginTurn();
		}
	}
	else
	{
		//print("not a player");
	}
}

void ALoutreWarsGameMode::OnPlayersTurnEnd()
{
	//print("Je switch de player");
	DisableNavigationWidget();
	SwitchCurrentPlayer();
}
