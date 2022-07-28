// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ChunkBase.h"
#include "Utils/FastNoiseLite.h"
#include "ProceduralMeshComponent.h"
#include "Game/HeightmapGenerator.h"
#include "Game/Water.h"
#include "Game/ObjectBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkBase::AChunkBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Noise = new FastNoiseLite();

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Mesh->SetCastShadow(true);
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AChunkBase::BeginPlay()
{
	Super::BeginPlay();

	Noise->SetFrequency(Frequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetFractalOctaves(3);

	Setup();

	//GenerateHeightMap();
	//
	//GenerateMesh();
	//
	//UE_LOG(LogTemp, Warning, TEXT("Vertex Count : %d"), VertexCount);
	//
	//ApplyMesh();

	Worker = new FHeightmapGenerator(GetActorLocation() / 100, Size, Noise, Continentalness, RandomOffset);
}

void AChunkBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Worker && !FinishedHMGeneration)
	{
		if (Worker->bStopThread)
		{
			Blocks = Worker->Blocks;

			GenerateMesh();

			ApplyMesh();
			FinishedHMGeneration = true;
			delete Worker;
			CreateWater();
		}
	}

}

void AChunkBase::GenerateHeightMap()
{
	switch (GenerationType)
	{
	case EGenerationType::GT_3D:
		Generate3DHeightMap(GetActorLocation() / 100);
		break;
	case EGenerationType::GT_2D:
		Generate2DHeightMap(GetActorLocation() / 100);
		break;
	default:
		throw std::invalid_argument("Invalid Generation Type");
	}
}

void AChunkBase::ApplyMesh() const
{
	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(
		0,
		MeshData.Vertices,
		MeshData.Triangles,
		MeshData.Normals,
		MeshData.UV0,
		MeshData.Colors,
		TArray<FProcMeshTangent>(),
		true
	);
}

void AChunkBase::ClearMesh()
{
	VertexCount = 0;
	MeshData.Clear();
}

void AChunkBase::SelfDestruct()
{
	Water->Destroy();
	Destroy();
}

void AChunkBase::CreateFoliage()
{
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			for (int z = 384 - 1; z >= 0; z--)
			{
				if (Blocks[GetBlockIndex(x, y, z)] == EBlock::Grass && z > 338)
				{
					if (FMath::FRandRange(1.f, 75.f) < 2)
					{
						auto transform = FTransform(
							FRotator::ZeroRotator,
							FVector(GetActorLocation().X + ((x - 2) * 100), GetActorLocation().Y + ((y - 2) * 100), z * 100),
							FVector::OneVector
						);

						auto Object = GetWorld()->SpawnActorDeferred<AObjectBase>(
							FoliageType,
							transform,
							this
							);

						UGameplayStatics::FinishSpawningActor(Object, transform);
					}
				}
			}
		}
	}
}

void AChunkBase::CreateWater()
{
	auto transform = FTransform(
		FRotator::ZeroRotator,
		GetActorLocation(),
		FVector::OneVector
	);

	 Water = GetWorld()->SpawnActorDeferred<AWater>(
		 WaterType,
		transform,
		this
		);

	Water->Size = Size;
	Water->MainBlocks = Blocks;

	UGameplayStatics::FinishSpawningActor(Water, transform);
}

int AChunkBase::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

EBlock AChunkBase::ModifyVoxel(const FIntVector Position, const EBlock Block)
{
	if (Position.X >= Size || Position.Y >= Size || Position.Z >= 384 || Position.X < 0 || Position.Y < 0 || Position.Z < 0)
	{
		return EBlock::Air;
	}
	EBlock TempBlock = Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)];

	ModifyVoxelData(Position, Block);

	ClearMesh();

	GenerateMesh();

	ApplyMesh();

	if (Block == EBlock::Air && Position.Z == 337)
	{
		Water->ModifyVoxel(Position);
	}

	return TempBlock;
}