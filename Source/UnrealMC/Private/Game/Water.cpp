// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Water.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AWater::AWater()
{
	WaterMesh = CreateDefaultSubobject<UProceduralMeshComponent>("WaterMesh");
	WaterMesh->SetCastShadow(true);
	SetRootComponent(WaterMesh);
}

// Called when the game starts or when spawned
void AWater::BeginPlay()
{
	Super::BeginPlay();

	Setup();
	GenerateWater();
	GenerateMesh();
	ApplyMesh();
}

void AWater::Setup()
{
	// Initialize Blocks
	WaterBlocks.SetNum(Size * Size * 384);
}

void AWater::GenerateWater()
{
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			for (int z = 0; z < 384; ++z)
			{
				if (z == 337)
				{
					if (MainBlocks[GetWaterBlockIndex(x, y, 337)] == EBlock::Air)
					{
						WaterBlocks[GetWaterBlockIndex(x, y, 337)] = EBlock::Water;
					}
					else
					{
						WaterBlocks[GetWaterBlockIndex(x, y, z)] = EBlock::Air;
					}
				}
				else
				{
					WaterBlocks[GetWaterBlockIndex(x, y, z)] = EBlock::Air;
				}
			}
		}
	}
}

void AWater::GenerateMesh()
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

					CurrentBlock = GetWaterBlock(ChunkItr);
					//CompareBlock = GetCompareWaterBlock(ChunkItr, ChunkItr + AxisMask);
					CompareBlock = GetWaterBlock(ChunkItr + AxisMask);

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

						if (Axis1Limit != 384 && Axis2Limit != 384)
						{
							CreateWaterQuad(
								CurrentMask, AxisMask, Width, Height,
								ChunkItr,
								ChunkItr + DeltaAxis1,
								ChunkItr + DeltaAxis2,
								ChunkItr + DeltaAxis1 + DeltaAxis2
							);
						}

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

void AWater::CreateWaterQuad(
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
	const auto Normal = FVector(AxisMask * Mask.Normal);
	const auto Color = FColor(255, 255, 255, 255);

	if (Normal.Z == 1)
	{
		WaterMeshData.Vertices.Append({
			(FVector(V1) * 100) - FVector(0, 0, 30),
			(FVector(V2) * 100) - FVector(0, 0, 30),
			(FVector(V3) * 100) - FVector(0, 0, 30),
			(FVector(V4) * 100) - FVector(0, 0, 30)
			});
	}
	else if (Normal.Z == -1)
	{
		WaterMeshData.Vertices.Append({
			(FVector(V1) * 100) + FVector(0, 0, 70),
			(FVector(V2) * 100) + FVector(0, 0, 70),
			(FVector(V3) * 100) + FVector(0, 0, 70),
			(FVector(V4) * 100) + FVector(0, 0, 70)
			});
	}
	else
	{
		WaterMeshData.Vertices.Append({
			FVector(V1) * 100,
			FVector(V2) * 100,
			FVector(V3) * 100,
			FVector(V4) * 100
			});
	}

	WaterMeshData.Triangles.Append({
		WaterVertexCount,
		WaterVertexCount + 2 + Mask.Normal,
		WaterVertexCount + 2 - Mask.Normal,
		WaterVertexCount + 3,
		WaterVertexCount + 1 - Mask.Normal,
		WaterVertexCount + 1 + Mask.Normal
		});

	WaterMeshData.Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
		});

	WaterMeshData.Colors.Append({
		Color,
		Color,
		Color,
		Color
		});

	WaterMeshData.UV0.Append({
	FVector2D(0, 0),
	FVector2D(0, Width),
	FVector2D(Height, 0),
	FVector2D(Height, Width)
		});

	WaterVertexCount += 4;
}

void AWater::ApplyMesh() const
{
	WaterMesh->SetMaterial(0, Material);
	WaterMesh->CreateMeshSection(
		0,
		WaterMeshData.Vertices,
		WaterMeshData.Triangles,
		WaterMeshData.Normals,
		WaterMeshData.UV0,
		WaterMeshData.Colors,
		TArray<FProcMeshTangent>(),
		true
	);
}

int AWater::GetWaterBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

EBlock AWater::GetWaterBlock(const FIntVector Index) const
{
	if (Index.X >= Size || Index.Y >= Size || Index.Z >= 384 || Index.X < 0 || Index.Y < 0 || Index.Z < 0)
		return EBlock::Air;
	return WaterBlocks[GetWaterBlockIndex(Index.X, Index.Y, Index.Z)];
}

bool AWater::CompareMask(const FMask M1, const FMask M2) const
{
	return M1.Block == M2.Block && M1.Normal == M2.Normal;
}

void AWater::ModifyVoxel(const FIntVector Position)
{
	ModifyVoxelData(Position);

	ClearMesh();

	GenerateMesh();

	ApplyMesh();
}

void AWater::ModifyVoxelData(const FIntVector Position)
{
	const int Index = GetWaterBlockIndex(Position.X, Position.Y, Position.Z);

	WaterBlocks[Index] = EBlock::Water;
}

void AWater::ClearMesh()
{
	WaterVertexCount = 0;
	WaterMeshData.Clear();
}
