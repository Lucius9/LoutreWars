// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "GridMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LOUTREWARS_API UGridMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement") float MovementRange = 4;
	
};
