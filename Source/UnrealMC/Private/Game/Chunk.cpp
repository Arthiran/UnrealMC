// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Chunk.h"
#include "Utils/FastNoiseLite.h"

void AChunk::Setup()
{
	// Initialize Blocks
	Blocks.SetNum(Size * Size * 384);
}

void AChunk::Generate2DHeightMap(const FVector Position)
{
	// A random seed for variation
	const int RandomSeed = FMath::RandRange(10000, 99999);

	// Think of this as a zoom level. Higher the frequency more detail but with decrease size.
	const float NewFrequency = 5.f;

	// Scale or intensity of the noise.
	const float Amplitude = 10.f;

	// If the noise value is greater than this then empty block/ Basically lower the value more caves.
	const float CaveThreshold = 0.425;

	for (int x = 0; x < Size; x++)
	{
		for (int y = 0; y < Size; y++)
		{
			const float Xpos = x + Position.X;
			const float ypos = y + Position.Y;

			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, ypos) + 1) * Size / 2), 0, Size) + 368;

			for (int z = 0; z < Height; z++)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
			}

			for (int z = Height; z < 384; z++)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
			}
		}
	}
}

void AChunk::Generate3DHeightMap(const FVector Position)
{
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			bool placedGrass = false;
			for (int z = 384 - 1; z >= 0; z--)
			{
				const float NoiseValue = Noise->GetNoise(x + Position.X, y + Position.Y, z + Position.Z);
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
				else
				{
					Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
				}
			}
		}
	}
}

void AChunk::GenerateMesh()
{
	// Sweep over each axis (X, Y, Z)
	for (int Axis = 0; Axis < 3; ++Axis)
	{
		// 2 Perpendicular axis
		const int Axis1 = (Axis + 1) % 3;
		const int Axis2 = (Axis + 2) % 3;

		int MainAxisLimit = Size;
		int Axis1Limit = Size;
		int Axis2Limit = Size;

		if (Axis == 2)
		{
			MainAxisLimit = 384;
		}
		else if (Axis1 == 2)
		{
			Axis1Limit = 384;
		}
		else if (Axis2 == 2)
		{
			Axis2Limit = 384;
		}

		auto DeltaAxis1 = FIntVector::ZeroValue;
		auto DeltaAxis2 = FIntVector::ZeroValue;

		auto ChunkItr = FIntVector::ZeroValue;
		auto AxisMask = FIntVector::ZeroValue;

		AxisMask[Axis] = 1;

		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit);

		// Check each slice of the chunk
		for (ChunkItr[Axis] = -1; ChunkItr[Axis] < MainAxisLimit;)
		{
			int N = 0;

			// Compute Mask
			for (ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis2Limit; ++ChunkItr[Axis2])
			{
				for (ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis1Limit; ++ChunkItr[Axis1])
				{
					EBlock CurrentBlock;
					EBlock CompareBlock;

					CurrentBlock = GetBlock(ChunkItr);
					CompareBlock = GetBlock(ChunkItr + AxisMask);

					const bool CurrentBlockOpaque = CurrentBlock != EBlock::Air;
					const bool CompareBlockOpaque = CompareBlock != EBlock::Air;

					if (CurrentBlockOpaque == CompareBlockOpaque)
					{
						Mask[N++] = FMask{ EBlock::Null, 0 };
					}
					else if (CurrentBlockOpaque)
					{
						Mask[N++] = FMask{ CurrentBlock, 1 };
					}
					else
					{
						Mask[N++] = FMask{ CompareBlock, -1 };
					}
				}
			}

			++ChunkItr[Axis];
			N = 0;

			// Generate Mesh From Mask
			for (int j = 0; j < Axis2Limit; ++j)
			{
				for (int i = 0; i < Axis1Limit;)
				{
					if (Mask[N].Normal != 0)
					{
						const auto CurrentMask = Mask[N];
						ChunkItr[Axis1] = i;
						ChunkItr[Axis2] = j;

						int Width;

						for (Width = 1; i + Width < Axis1Limit && CompareMask(Mask[N + Width], CurrentMask); ++Width)
						{
						}

						int Height;
						bool Done = false;

						for (Height = 1; j + Height < Axis2Limit; ++Height)
						{
							for (int k = 0; k < Width; ++k)
							{
								if (CompareMask(Mask[N + k + Height * Axis1Limit], CurrentMask)) continue;

								Done = true;
								break;
							}

							if (Done) break;
						}

						DeltaAxis1[Axis1] = Width;
						DeltaAxis2[Axis2] = Height;

						CreateQuad(
							CurrentMask, AxisMask, Width, Height,
							ChunkItr,
							ChunkItr + DeltaAxis1,
							ChunkItr + DeltaAxis2,
							ChunkItr + DeltaAxis1 + DeltaAxis2
						);

						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for (int l = 0; l < Height; ++l)
						{
							for (int k = 0; k < Width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = FMask{ EBlock::Null, 0 };
							}
						}

						i += Width;
						N += Width;
					}
					else
					{
						i++;
						N++;
					}
				}
			}
		}
	}
}

void AChunk::CreateQuad(
	const FMask Mask,
	const FIntVector AxisMask,
	const int Width,
	const int Height,
	const FIntVector V1,
	const FIntVector V2,
	const FIntVector V3,
	const FIntVector V4
)
{
	float XOffset = 0;
	float YOffset = 0;

	const auto Normal = FVector(AxisMask * Mask.Normal);

	if (Mask.Block == EBlock::OakLeaves)
	{
		XOffset = 7;
		YOffset = 0;
	}
	else if (Mask.Block == EBlock::Oak)
	{
		if (Normal.Z == 1 || Normal.Z == -1)
		{
			XOffset = 6;
			YOffset = 0;
		}
		else
		{
			XOffset = 5;
			YOffset = 0;
		}
	}
	else if (Mask.Block == EBlock::Grass)
	{
		if (Normal.Z == 1)
		{
			XOffset = 1;
			YOffset = 0;
		}
		else if (Normal.Z == -1)
		{
			XOffset = 2;
			YOffset = 0;
		}
		else
		{
			XOffset = 0;
			YOffset = 0;
		}
	}
	else if (Mask.Block == EBlock::Dirt)
	{
		XOffset = 3;
		YOffset = 0;
	}
	else if (Mask.Block == EBlock::Stone)
	{
		XOffset = 4;
		YOffset = 0;
	}
	else if (Mask.Block == EBlock::Coal)
	{
		XOffset = 0;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Iron)
	{
		XOffset = 1;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Lapis)
	{
		XOffset = 2;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Gold)
	{
		XOffset = 3;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Diamond)
	{
		XOffset = 4;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Redstone)
	{
		XOffset = 5;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Brown_Concrete)
	{
		XOffset = 6;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Purple_Concrete)
	{
		XOffset = 7;
		YOffset = 1;
	}
	else if (Mask.Block == EBlock::Gray_Concrete)
	{
		XOffset = 0;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Cyan_Concrete)
	{
		XOffset = 1;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Green_Concrete)
	{
		XOffset = 2;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Lime_Concrete)
	{
		XOffset = 3;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Orange_Concrete)
	{
		XOffset = 4;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Magenta_Concrete)
	{
		XOffset = 5;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Light_Blue_Concrete)
	{
		XOffset = 6;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Pink_Concrete)
	{
		XOffset = 7;
		YOffset = 2;
	}
	else if (Mask.Block == EBlock::Blue_Concrete)
	{
		XOffset = 0;
		YOffset = 3;
	}
	else if (Mask.Block == EBlock::Light_Gray_Concrete)
	{
		XOffset = 1;
		YOffset = 3;
	}
	else if (Mask.Block == EBlock::White_Concrete)
	{
		XOffset = 2;
		YOffset = 3;
	}
	else if (Mask.Block == EBlock::Black_Concrete)
	{
		XOffset = 3;
		YOffset = 3;
	}
	else if (Mask.Block == EBlock::Yellow_Concrete)
	{
		XOffset = 4;
		YOffset = 3;
	}
	else if (Mask.Block == EBlock::Red_Concrete)
	{
		XOffset = 5;
		YOffset = 3;
	}
	
	const auto Color = FColor(XOffset, YOffset, Width, Height);

	MeshData.Vertices.Append({
		FVector(V1) * 100,
		FVector(V2) * 100,
		FVector(V3) * 100,
		FVector(V4) * 100
		});

	MeshData.Triangles.Append({
		VertexCount,
		VertexCount + 2 + Mask.Normal,
		VertexCount + 2 - Mask.Normal,
		VertexCount + 3,
		VertexCount + 1 - Mask.Normal,
		VertexCount + 1 + Mask.Normal
		});

	MeshData.Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
		});

	MeshData.Colors.Append({
		Color,
		Color,
		Color,
		Color
		});

	MeshData.UV0.Append({
	FVector2D(0, 0),
	FVector2D(0, Width),
	FVector2D(Height, 0),
	FVector2D(Height, Width)
		});

	VertexCount += 4;
}

void AChunk::ModifyVoxelData(const FIntVector Position, const EBlock Block)
{
	const int Index = GetBlockIndex(Position.X, Position.Y, Position.Z);

	Blocks[Index] = Block;
}

int AChunk::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

EBlock AChunk::GetBlock(const FIntVector Index) const
{
	if (Index.X >= Size || Index.Y >= Size || Index.Z >= 384 || Index.X < 0 || Index.Y < 0 || Index.Z < 0)
		return EBlock::Air;
	return Blocks[GetBlockIndex(Index.X, Index.Y, Index.Z)];
}

bool AChunk::CompareMask(const FMask M1, const FMask M2) const
{
	return M1.Block == M2.Block && M1.Normal == M2.Normal;
}