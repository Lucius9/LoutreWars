// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Faction/Faction.h"
#include "GridAIController.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API AGridAIController : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	AFaction *Faction;
	
public:
	AGridAIController(const FObjectInitializer &Initializer);

public :	
	UFUNCTION(BlueprintCallable, Category = "AI Turn Management")
	virtual void BeginTurn();
	UFUNCTION(BlueprintCallable,Category="AI Turn Management")
	virtual void EndTurn();

	DECLARE_EVENT(AAIController, FOnAITurnEnd);
	FOnAITurnEnd OnAITurnEnd() { return TurnEndEvent; };

private:
	FOnAITurnEnd TurnEndEvent;
	
};
