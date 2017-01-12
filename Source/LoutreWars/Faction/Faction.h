// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Faction.generated.h"

UCLASS()
class LOUTREWARS_API AFaction : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Faction Parameter")
	FString FactionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Parameter")
	TSubclassOf<AController> ControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Parameter")
	uint8 TeamIndex;	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Faction Parameter")
	bool isPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Parameter", meta = (EditCondition = isPlayerController))
	int32 PlayerNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Faction Parameter")
	AController *Controller;
	
public:	
	// Sets default values for this actor's properties
	AFaction();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

};
