// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "GridPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API AGridPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int TeamIndex;
	
	
};
