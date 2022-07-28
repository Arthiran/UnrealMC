// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HeightmapGenerator.h"
#include "Utils/FastNoiseLite.h"
#include "Game/Foliage.h"
#include "Utils/PRNG.h"

FHeightmapGenerator::FHeightmapGenerator(FVector InPosition, int InSize, FastNoiseLite* InNoise, UCurveFloat* InCurve, FVector InRandomOffset)
{
	Position = InPosition;
	Size = InSize;
	Noise = InNoise;
	Continentalness = InCurve;
	RandomOffset = InRandomOffset;
	// Constructs the actual thread object. It will begin execution immediately
	// If you've passed in any inputs, set them up before calling this.
	Thread = FRunnableThread::Create(this, TEXT("HeightmapThread"));
}


FHeightmapGenerator::~FHeightmapGenerator()
{
	if (Thread != nullptr)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

bool FHeightmapGenerator::Init()
{
	// Initialize Blocks
	Blocks.SetNum(Size * Size * 384);
	rng = new PRNG();
	// Return false if you want to abort the thread
	return true;
}

uint32 FHeightmapGenerator::Run()
{
	bStopThread = false;
	// Peform your processor intensive task here. In this example, a neverending
	// task is created, which will only end when Stop is called.
	while (!bStopThread)
	{
		for (int x = 0; x < Size; ++x)
		{
			for (int y = 0; y < Size; ++y)
			{
				bool placedGrass = false;
				for (int z = 384 - 1; z >= 0; z--)
				{
					const float NoiseValue = Noise->GetNoise(x + Position.X + RandomOffset.X, y + Position.Y + RandomOffset.Y, z + Position.Z + RandomOffset.Z);
					if (NoiseValue + Continentalness->GetFloatValue(z) >= 0)
					{
						if (z > 320)
						{
							if (!placedGrass)
							{
								Blocks[GetBlockIndex(x, y, z)] = EBlock::Grass;
								placedGrass = true;
							}
							else
							{
								Blocks[GetBlockIndex(x, y, z)] = EBlock::Dirt;
							}
						}
						else
						{
							Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
						}
					}
					//else if (z == 335)
					//{
					//	Blocks[GetBlockIndex(x, y, z)] = EBlock::Water;
					//}
					else
					{
						Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
					}
				}
			}
		}

		for (int x = 0; x < Size; ++x)
		{
			for (int y = 0; y < Size; ++y)
			{
				for (int z = 384 - 1; z >= 0; z--)
				{
					if (Blocks[GetBlockIndex(x, y, z)] == EBlock::Grass && (z > 335 && z < 375 && x > 2 && x < 13 && y > 2 && y < 13))
					{
						if (rng->getRandomFloat(1.f, 50.f) > 49.f)
						{
							for (FBlockDefinition BlockDef : FFoliage::OakTreeSelection())
							{
								if (Blocks[GetBlockIndex(x + BlockDef.Position.X, y + BlockDef.Position.Y, z + BlockDef.Position.Z)] == EBlock::Air)
								{
									Blocks[GetBlockIndex(x + BlockDef.Position.X, y + BlockDef.Position.Y, z + BlockDef.Position.Z)] = BlockDef.BlockType;
								}
							}
						}
					}
				}
			}
		}

		// Do this once you've finished using the input/output variables
		// From this point onwards, do not touch them!
		bStopThread = true;

		// I hear that it's good to let the thread sleep a bit, so the OS can utilise it better or something.
		//FPlatformProcess::Sleep(0.01f);
	}

	return 0;
}


// This function is NOT run on the new thread!
void FHeightmapGenerator::Stop()
{
	// Clean up memory usage here, and make sure the Run() function stops soon
	// The main thread will be stopped until this finishes!

	// For this example, we just need to terminate the while loop
	// It will finish in <= 1 sec, due to the Sleep()
	bStopThread = false;
}

int FHeightmapGenerator::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}