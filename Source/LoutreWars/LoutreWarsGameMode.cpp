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
	AddTransitionWidget();
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
		CreatePlayer(Faction);
	}
}

void ALoutreWarsGameMode::CreatePlayer(AFaction *Faction)
{
	UWorld *World = GetWorld();
	AGridPawnPlayerController *PlayerController = Cast<AGridPawnPlayerController>(World->SpawnActor(Faction->ControllerClass));
	if (PlayerController)
	{
		PlayerController->Faction = Faction;
		PlayerController->InitPawns();
		Faction->Controller = PlayerController;
		UGameInstance *GameInstance = World->GetGameInstance();
		FString OutError;
		ULocalPlayer *NewPlayer = GameInstance->CreateLocalPlayer(PlayerController->GetUniqueID(), OutError, false);
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
	ALWWorldSettings *WorldSettings = Cast<ALWWorldSettings>(GetWorldSettings());
	if (WorldSettings && !WorldSettings->bRandomPlayersOrder)
	{
		for (AFaction *Faction : WorldSettings->PlayersOrder)
		{
			PlayersOrder.Add(Faction->Controller);
		}
		
	}
	else if (WorldSettings && WorldSettings->bRandomPlayersOrder && WorldSettings->bHumanPlayersFirst)
	{
		TArray<AGridPawnPlayerController*> HumanPlayers;
		TArray<AGridAIController*> AIPlayers;
		for (TActorIterator<AFaction> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->Controller->GetClass()->IsChildOf(AGridPawnPlayerController::StaticClass()))
			{
				HumanPlayers.Add(Cast<AGridPawnPlayerController>(ActorItr->Controller));
			}			
			else if(ActorItr->Controller->GetClass()->IsChildOf(AGridAIController::StaticClass()))
			{
				AIPlayers.Add(Cast<AGridAIController>(ActorItr->Controller));
			}			
		}
		while (HumanPlayers.Num() > 0)
		{
			int j = FMath::RandRange(0, HumanPlayers.Num() - 1);
			PlayersOrder.Add(HumanPlayers[j]);
			HumanPlayers.RemoveAtSwap(j);
		}
		while (AIPlayers.Num() > 0)
		{
			int j = FMath::RandRange(0, AIPlayers.Num() - 1);
			PlayersOrder.Add(AIPlayers[j]);
			AIPlayers.RemoveAtSwap(j);
		}
	}
	else
	{
		TArray<AController *> ArrayToShuffle;
		for (TActorIterator<AFaction> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ArrayToShuffle.Add(ActorItr->Controller);
		}
		while (ArrayToShuffle.Num() > 0)
		{
			int j = FMath::RandRange(0, ArrayToShuffle.Num() - 1);
			PlayersOrder.Add(ArrayToShuffle[j]);
			ArrayToShuffle.RemoveAtSwap(j);
		}
	}
	CurrentPlayerIndex = 0;
}

void ALoutreWarsGameMode::BeginTurnCurrentPlayer()
{
	if (IsCurrentPlayerHuman())
	{		
		AGridPawnPlayerController *PlayerController = Cast<AGridPawnPlayerController>(PlayersOrder[CurrentPlayerIndex]);
		EnableNavigationWidget();
		PlayerController->BeginTurn();
	}
	else
	{
		AGridAIController *AIController = Cast<AGridAIController>(PlayersOrder[CurrentPlayerIndex]);
		AIController->BeginTurn();
	}
}

AFaction* ALoutreWarsGameMode::GetCurrentPlayerFaction()
{
	if (IsCurrentPlayerAI())
	{
		AGridAIController *AIController = Cast<AGridAIController>(PlayersOrder[CurrentPlayerIndex]);
		return AIController->Faction;
	}
	else if (IsCurrentPlayerHuman())
	{
		AGridPawnPlayerController *PlayerController = Cast<AGridPawnPlayerController>(PlayersOrder[CurrentPlayerIndex]);
		return PlayerController->Faction;
	}
	return NULL;
}

bool ALoutreWarsGameMode::IsCurrentPlayerAI()
{
	AController *Controller = PlayersOrder[CurrentPlayerIndex];
	if (Controller->GetClass()->IsChildOf(AGridAIController::StaticClass()))
	{
		return true;
	}
	return false;
}

bool ALoutreWarsGameMode::IsCurrentPlayerHuman()
{
	AController *Controller = PlayersOrder[CurrentPlayerIndex];
	if (Controller->GetClass()->IsChildOf(AGridPawnPlayerController::StaticClass()))
	{
		return true;
	}
	return false;
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

void ALoutreWarsGameMode::AddTransitionWidget()
{
	UUserWidget *TransitionWidget=CreateWidget<UUserWidget>(GetWorld(), TransitionWidgetClass);
	TransitionWidget->AddToViewport();
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
}

void ALoutreWarsGameMode::OnPlayersTurnEnd()
{
	//print("Je switch de player");
	DisableNavigationWidget();
	SwitchCurrentPlayer();
	AddTransitionWidget();
}
