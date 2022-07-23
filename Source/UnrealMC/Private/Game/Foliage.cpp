// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Foliage.h"

TArray<FBlockDefinition> FFoliage::OakTreeSelection()
{
	switch (FMath::RandRange(0, 1))
	{
	case 0:
		return OakTree1();
	case 1:
		return OakTree2();
	default:
		return OakTree1();
	}
}

TArray<FBlockDefinition> FFoliage::OakTree1()
{
	TArray<FBlockDefinition> Blocks = TArray<FBlockDefinition>();
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 1), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 2), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 3), EBlock::Oak));

	// First Layer
	Blocks.Add(FBlockDefinition(FIntVector(-2, -2, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, -1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 0, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 2, 4), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(-1, -2, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, -1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 0, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 2, 4), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(0, -2, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, -1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 4), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 2, 4), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(1, -2, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, -1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 0, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 2, 4), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(2, -2, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, -1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 0, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 1, 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 2, 4), EBlock::OakLeaves));

	// Second Layer
	Blocks.Add(FBlockDefinition(FIntVector(-1, -1, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 0, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 1, 5), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(0, -1, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 5), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 1, 5), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(1, -1, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 0, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 1, 5), EBlock::OakLeaves));

	// Last Layer
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 6), EBlock::OakLeaves));
	return Blocks;
}

TArray<FBlockDefinition> FFoliage::OakTree2()
{
	TArray<FBlockDefinition> Blocks = TArray<FBlockDefinition>();
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 1), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 2), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 3), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 4), EBlock::Oak));

	// First Layer
	Blocks.Add(FBlockDefinition(FIntVector(-1, -1, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 0, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 1, 5), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(0, -1, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 5), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 1, 5), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(1, -1, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 0, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 1, 5), EBlock::OakLeaves));

	// Second Layer
	Blocks.Add(FBlockDefinition(FIntVector(-2, -2, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, -1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 0, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 2, 6), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(-1, -2, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, -1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 0, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 2, 6), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(0, -2, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, -1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 6), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 2, 6), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(1, -2, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, -1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 0, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 2, 6), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(2, -2, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, -1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 0, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 1, 6), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 2, 6), EBlock::OakLeaves));

	// Third Layer
	Blocks.Add(FBlockDefinition(FIntVector(-2, -2, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, -1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 0, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 2, 7), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(-1, -2, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, -1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 0, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 2, 7), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(0, -2, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, -1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 7), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 2, 7), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(1, -2, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, -1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 0, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 2, 7), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(2, -2, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, -1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 0, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 1, 7), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 2, 7), EBlock::OakLeaves));

	// Fourth Layer
	Blocks.Add(FBlockDefinition(FIntVector(-2, -2, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, -1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 0, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-2, 2, 8), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(-1, -2, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, -1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 0, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 2, 8), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(0, -2, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, -1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 8), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(0, 1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 2, 8), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(1, -2, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, -1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 0, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 2, 8), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(2, -2, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, -1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 0, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 1, 8), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(2, 2, 8), EBlock::OakLeaves));

	// Last Layer
	Blocks.Add(FBlockDefinition(FIntVector(-1, -1, 9), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 0, 9), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(-1, 1, 9), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(0, -1, 9), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 0, 9), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(0, 1, 9), EBlock::OakLeaves));

	Blocks.Add(FBlockDefinition(FIntVector(1, -1, 9), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 0, 9), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(1, 1, 9), EBlock::OakLeaves));

	return Blocks;
}