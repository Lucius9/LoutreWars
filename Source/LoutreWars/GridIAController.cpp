// Fill out your copyright notice in the Description page of Project Settings.

#include "LoutreWars.h"
#include "GridIAController.h"




AGridIAController::AGridIAController(const FObjectInitializer &Initializer)
	:Super(Initializer)
{
	this->bWantsPlayerState = true;
};