// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "UserWidget.h"
#include "HUD/NavigationWidget.h"
#include "LoutreWarsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API ALoutreWarsGameMode : public AGameMode
{
	GENERATED_BODY()
	
	
public:
	ALoutreWarsGameMode();
	virtual void BeginPlay()override;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeActionWidget(TSubclassOf<UUserWidget> NewActionWidgetClass);	
	void EnableMovementWidget();
	void EnableEndMovementWidget();
	void EnableAttackWidget();
	void HideWidget();

	void EnableNavigationWidget();
	void DisableNavigationWidget();
	void UpdateTileWidget();
	void UpdatePawnWidget();
	void HidePawnWidget();

protected:
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> MovementWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> AttackWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> EndMovementWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UNavigationWidget> NavigationWidgetClass;		

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	UUserWidget* CurrentActionWidget;

	UPROPERTY()
	UNavigationWidget *NavigationWidget;
	
};
