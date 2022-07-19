// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/Enums.h"
#include "BlockDefinition.generated.h"

USTRUCT(BlueprintType)
struct FBlockDefinition
{
	GENERATED_BODY()
	
public:	
	FBlockDefinition();
	FBlockDefinition(FIntVector InOffset, EBlock InBlockType)
		: Position(InOffset), BlockType(InBlockType)
	{}
	
	FIntVector Position;
	EBlock BlockType;
};