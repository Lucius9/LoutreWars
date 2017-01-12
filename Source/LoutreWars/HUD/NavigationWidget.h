// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Visual.h"
#include "GridPawnPlayerController.h"
#include "NavigationWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API UNavigationWidget : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *CurrentTileWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *CurrentPawnWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *ActionWidget;

public:
	virtual void NativeConstruct()override;
	UFUNCTION(BlueprintNativeEvent, Category = "Tile Widget")
	void UpdateTileWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "Tile Widget")
	void HideTileWidget();

	UFUNCTION(BlueprintNativeEvent, Category = "Pawn Widget")
	void UpdatePawnWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "Pawn Widget")
	void HidePawnWidget();

	UFUNCTION(BlueprintNativeEvent, Category = "Action Widget")
	void EnableMovementWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "Action Widget")
	void EnableEndMovementWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "Action Widget")
	void EnableAttackWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "Action Widget")
	void DisableActionWidget();
};