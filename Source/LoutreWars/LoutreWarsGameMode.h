// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "UserWidget.h"
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

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void EnableMovementWidget();
	void EnableEndMovementWidget();

protected:
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> MovementWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> AttackWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> EndMovementWidgetClass;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> ValidationWidgetClass;*/

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	UUserWidget* CurrentWidget;
	
};
