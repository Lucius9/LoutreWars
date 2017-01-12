// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "LWWorldSettings.generated.h"

/**
 * 
 */

UCLASS()
class LOUTREWARS_API ALWWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loutre Wars Game")
	uint8 NumberOfFactions = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loutre Wars Game")
	uint8 NumberOfPlayer = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loutre Wars Game")
	uint8 NumberOfAI = 1;
};
