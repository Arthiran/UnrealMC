// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ObjectBase.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AObjectBase::AObjectBase()
{
	ObjectMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ObjectMesh");
	ObjectMesh->SetCastShadow(true);
	SetRootComponent(ObjectMesh);
}

// Called when the game starts or when spawned
void AObjectBase::BeginPlay()
{
	Super::BeginPlay();

	Setup();
	GenerateObject();
	GenerateMesh();
	ApplyMesh();
}

void AObjectBase::Setup()
{
	// Initialize Blocks
	ObjectBlocks.SetNum(5 * 5 * 10);
}

void AObjectBase::GenerateObject()
{
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			for (int z = 0; z < 10; ++z)
			{
				bool bFoundBlock = false;
				for (FBlockDefinition BlockDef : ObjectDefinition(FIntVector(2, 2, 0)))
				{
					if (BlockDef.Position == FIntVector(x, y, z))
					{
						bFoundBlock = true;
						ObjectBlocks[GetObjectBlockIndex(x, y, z)] = BlockDef.BlockType;
						break;
					}
				}

				if (!bFoundBlock)
				{
					ObjectBlocks[GetObjectBlockIndex(x, y, z)] = EBlock::Air;
				}
			}
		}
	}
}

void AObjectBase::GenerateMesh()
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
			MainAxisLimit = 10;
		}
		else if (Axis1 == 2)
		{
			Axis1Limit = 10;
		}
		else if (Axis2 == 2)
		{
			Axis2Limit = 10;
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

					CurrentBlock = GetObjectBlock(ChunkItr);
					//CompareBlock = GetCompareObjectBlock(ChunkItr, ChunkItr + AxisMask);
					CompareBlock = GetObjectBlock(ChunkItr + AxisMask);

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

						CreateObjectQuad(
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

void AObjectBase::CreateObjectQuad(
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
	int alpha = 0;

	if (Mask.Block == EBlock::OakLeaves)
	{
		alpha = 255;
	}
	else if (Mask.Block == EBlock::Stone)
	{
		alpha = 0;
	}

	const auto Normal = FVector(AxisMask * Mask.Normal);
	const auto Color = FColor(255, 0, 0, alpha);
	const auto UpColor = FColor(0, 255, 0, alpha);

	ObjectMeshData.Vertices.Append({
		FVector(V1) * 100,
		FVector(V2) * 100,
		FVector(V3) * 100,
		FVector(V4) * 100
	});

	ObjectMeshData.Triangles.Append({
		ObjectVertexCount,
		ObjectVertexCount + 2 + Mask.Normal,
		ObjectVertexCount + 2 - Mask.Normal,
		ObjectVertexCount + 3,
		ObjectVertexCount + 1 - Mask.Normal,
		ObjectVertexCount + 1 + Mask.Normal
		});

	ObjectMeshData.Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
		});

	if (Normal.Z == 1)
	{
		ObjectMeshData.Colors.Append({
			UpColor,
			UpColor,
			UpColor,
			UpColor
			});
	}
	else if (Normal.Z == -1)
	{
		ObjectMeshData.Colors.Append({
			UpColor,
			UpColor,
			UpColor,
			UpColor
			});
	}
	else
	{
		ObjectMeshData.Colors.Append({
			Color,
			Color,
			Color,
			Color
			});
	}

	ObjectMeshData.UV0.Append({
	FVector2D(0, 0),
	FVector2D(0, Width),
	FVector2D(Height, 0),
	FVector2D(Height, Width)
		});

	ObjectVertexCount += 4;
}

void AObjectBase::ApplyMesh() const
{
	ObjectMesh->SetMaterial(0, Material);
	ObjectMesh->CreateMeshSection(
		0,
		ObjectMeshData.Vertices,
		ObjectMeshData.Triangles,
		ObjectMeshData.Normals,
		ObjectMeshData.UV0,
		ObjectMeshData.Colors,
		TArray<FProcMeshTangent>(),
		true
	);
}

int AObjectBase::GetObjectBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

EBlock AObjectBase::GetObjectBlock(const FIntVector Index) const
{
	if (Index.X >= Size || Index.Y >= Size || Index.Z >= 10 || Index.X < 0 || Index.Y < 0 || Index.Z < 0)
		return EBlock::Air;
	return ObjectBlocks[GetObjectBlockIndex(Index.X, Index.Y, Index.Z)];
}

bool AObjectBase::CompareMask(const FMask M1, const FMask M2) const
{
	return M1.Block == M2.Block && M1.Normal == M2.Normal;
}

void AObjectBase::ModifyVoxel(const FIntVector Position, const EBlock Block)
{
	ModifyVoxelData(Position, Block);

	ClearMesh();

	GenerateMesh();

	ApplyMesh();
}

void AObjectBase::ModifyVoxelData(const FIntVector Position, const EBlock Block)
{
	const int Index = GetObjectBlockIndex(Position.X, Position.Y, Position.Z);

	ObjectBlocks[Index] = Block;
}

void AObjectBase::ClearMesh()
{
	ObjectVertexCount = 0;
	ObjectMeshData.Clear();
}

TArray<FBlockDefinition> AObjectBase::ObjectDefinition(const FIntVector Offset)
{
	TArray<FBlockDefinition> Blocks = TArray<FBlockDefinition>();
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 1), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 2), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 3), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 1), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 2), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 3), EBlock::Oak));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 2, Offset.Y + 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 2, Offset.Y + 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 2, Offset.Y, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 2, Offset.Y - 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 2, Offset.Y - 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y + 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y + 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y - 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y - 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y + 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y + 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y - 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y - 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y + 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y + 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y - 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y - 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 2, Offset.Y + 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 2, Offset.Y + 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 2, Offset.Y, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 2, Offset.Y - 1, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 2, Offset.Y - 2, Offset.Z + 4), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y - 1, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X - 1, Offset.Y + 1, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y - 1, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y + 1, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y - 1, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X + 1, Offset.Y + 1, Offset.Z + 5), EBlock::OakLeaves));
	Blocks.Add(FBlockDefinition(FIntVector(Offset.X, Offset.Y, Offset.Z + 6), EBlock::OakLeaves));
	return Blocks;
}