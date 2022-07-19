// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Utils/Enums.h"

class FastNoiseLite;

class FHeightmapGenerator : public FRunnable
{	
public:	
	// Sets default values for this actor's properties
	FHeightmapGenerator(FVector InPosition, int InSize, FastNoiseLite* InNoise, UCurveFloat* InCurve, FVector InRandomOffset);
	// Destructor
	virtual ~FHeightmapGenerator() override;


	// Overriden from FRunnable
	// Do not call these functions youself, that will happen automatically
	bool Init() override; // Do your setup here, allocate memory, ect.
	uint32 Run() override; // Main data processing happens here
	void Stop() override; // Clean up any memory you allocated here

	// The boolean that acts as the main switch
	// When this is false, inputs and outputs can be safely read from game thread


	// Declare the variables that are the inputs and outputs here.
	// You can have as many as you want. Remember not to use pointers, this must be
	// plain old data
	// For example:
	FVector Position = FVector::ZeroVector;
	int Size = 16;
	FastNoiseLite* Noise;
	UCurveFloat* Continentalness;
	FVector RandomOffset = FVector::ZeroVector;

	TArray<EBlock> Blocks;
	bool bStopThread = false;

private:

	// Thread handle. Control the thread using this, with operators like Kill and Suspend
	FRunnableThread* Thread = nullptr;

	// Used to know when the thread should exit, changed in Stop(), read in Run()

	int GetBlockIndex(int X, int Y, int Z) const;
};
