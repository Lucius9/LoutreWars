// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "UserWidget.h"
#include "HUD/NavigationWidget.h"
#include "GridPawnPlayerController.h"
#include "GridAIController.h"
#include "LoutreWarsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API ALoutreWarsGameMode : public AGameMode
{
	GENERATED_BODY()

public :
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Turn Management")
	TArray<AController*> PlayersOrder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Management")
	int CurrentPlayerIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Management")
	int TurnNumber=1;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UNavigationWidget> NavigationWidgetClass;

	UPROPERTY()
	UNavigationWidget *NavigationWidget;
	
public:
	ALoutreWarsGameMode();
	virtual void BeginPlay()override;

	void InitPlayer(AFaction *Faction);
	void InitAI(AFaction *Faction);
	void InitPlayerOrder();

public :	
	void EnableMovementWidget();
	void EnableEndMovementWidget();
	void EnableAttackWidget();	
	void DisableActionWidget();

	void EnableNavigationWidget();
	void DisableNavigationWidget();
	void UpdateTileWidget();
	void HideTileWidget();
	void UpdatePawnWidget();
	void HidePawnWidget();	

public :
	UFUNCTION(BlueprintCallable, Category = "Turn Management")
	virtual void SwitchCurrentPlayer();
	UFUNCTION(Category="Turn Management")
	void OnPlayersTurnEnd();
	
	
public:
	DECLARE_EVENT(ALoutreWarsGameMode,FOnCurrentPlayerChanged)	
	FOnCurrentPlayerChanged OnCurrentPlayerChanged() { return CurrentPlayerChangedEvent; };

private:
	FOnCurrentPlayerChanged CurrentPlayerChangedEvent;
};
