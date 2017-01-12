// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridAIController.h"




AGridAIController::AGridAIController(const FObjectInitializer &Initializer)
	:Super(Initializer)
{
	this->bWantsPlayerState = true;
};

void AGridAIController::BeginTurn()
{

}
void AGridAIController::EndTurn()
{
	TurnEndEvent.Broadcast();
}