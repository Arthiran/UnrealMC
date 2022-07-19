// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ChunkWorld.h"
#include "Game/ChunkBase.h"
#include "Utils/FastNoiseLite.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	FullChunkCount = FMath::Pow((DrawDistance * 2) + 1, 2);

	RandomOffset.X = FMath::RandRange(-100.f, 100.f) * 10000.f;
	RandomOffset.Y = FMath::RandRange(-100.f, 100.f) * 10000.f;
	RandomOffset.Z = FMath::RandRange(-100.f, 100.f) * 10000.f;
	Generate3DWorld();
	bIsStarted = true;
}

void AChunkWorld::SpawnChunk()
{
	if (ChunkQueue.IsEmpty())
	{
		if (bIsStarted)
		{
			bIsStarted = false;
		}

		if (GetWorldTimerManager().IsTimerActive(SpawnChunksHandle))
		{
			RemoveChunks(CurrentPlayerPosition);
			GetWorldTimerManager().ClearTimer(SpawnChunksHandle);
		}
		return;
	}

	FTransform OutForm;
	ChunkQueue.Dequeue(OutForm);

	if (CurrentChunks.Find(OutForm.GetLocation()))
	{
		return;
	}

	const auto chunk = GetWorld()->SpawnActorDeferred<AChunkBase>(
		ChunkType,
		OutForm,
		this
		);
	
	chunk->GenerationType = EGenerationType::GT_3D;
	chunk->Frequency = Frequency;
	chunk->Material = Material;
	chunk->Size = Size;
	chunk->Continentalness = Continentalness;
	chunk->RandomOffset = RandomOffset;
	
	UGameplayStatics::FinishSpawningActor(chunk, OutForm);
	CurrentChunks.Add(FChunkVector(OutForm.GetLocation()), chunk);
}

void AChunkWorld::RemoveChunks(const FIntVector PlayerPosition)
{
	TMap<FChunkVector, AChunkBase*> TempCurrentChunks = CurrentChunks;
	for (TPair<FChunkVector, AChunkBase*> Chunk : TempCurrentChunks)
	{
		if (Chunk.Value)
		{
			if (Chunk.Value->GetActorLocation().X < ((-DrawDistance + PlayerPosition.X) * Size * 100) || Chunk.Value->GetActorLocation().X > ((DrawDistance + PlayerPosition.X) * Size * 100) || Chunk.Value->GetActorLocation().Y < ((-DrawDistance + PlayerPosition.Y) * Size * 100) || Chunk.Value->GetActorLocation().Y > ((DrawDistance + PlayerPosition.Y) * Size * 100))
			{
				Chunk.Value->SelfDestruct();
				CurrentChunks.Remove(Chunk.Key);
			}
		}
		else
		{
			CurrentChunks.Remove(Chunk.Key);
		}
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ChunkType, Actors);

	for (AActor* TempActor : Actors)
	{
		if (AChunkBase* Chunky = Cast<AChunkBase>(TempActor))
		{
			if (Chunky)
			{
				if (Chunky->GetActorLocation().X < ((-DrawDistance + PlayerPosition.X) * Size * 100) || Chunky->GetActorLocation().X >((DrawDistance + PlayerPosition.X) * Size * 100) || Chunky->GetActorLocation().Y < ((-DrawDistance + PlayerPosition.Y) * Size * 100) || Chunky->GetActorLocation().Y >((DrawDistance + PlayerPosition.Y) * Size * 100))
				{
					Chunky->SelfDestruct();
				}
			}
		}
	}
}

EBlock AChunkWorld::FindAndModifyChunk(const FIntVector BlockPosition, const FIntVector LocalPosition, const EBlock Block)
{
	for (TPair<FChunkVector, AChunkBase*> Chunk : CurrentChunks)
	{
		if (BlockPosition.X >= Chunk.Value->GetActorLocation().X && BlockPosition.X <= Chunk.Value->GetActorLocation().X + 1600)
		{
			if (BlockPosition.Y >= Chunk.Value->GetActorLocation().Y && BlockPosition.Y <= Chunk.Value->GetActorLocation().Y + 1600)
			{
				Chunk.Value->ModifyVoxel(LocalPosition, Block);
				return Block;
			}
		}
	}

	return EBlock::Air;
}

void AChunkWorld::UpdateChunks(const FIntVector PlayerPosition)
{
	CurrentPlayerPosition = PlayerPosition;
	for (int x = -DrawDistance; x <= DrawDistance; x++)
	{
		for (int y = -DrawDistance; y <= DrawDistance ; y++)
		{
			auto transform = FTransform(
				FRotator::ZeroRotator,
				FVector((x + PlayerPosition.X) * Size * 100, (y + PlayerPosition.Y) * Size * 100, 0),
				FVector::OneVector
			);

			if (CurrentChunks.Find(transform.GetLocation()))
			{
				ChunkCount++;
				continue;
			}

			ChunkQueue.Enqueue(transform);

			if (!ChunkQueue.IsEmpty())
			{
				if (!GetWorldTimerManager().IsTimerActive(SpawnChunksHandle))
				{
					GetWorldTimerManager().SetTimer(SpawnChunksHandle, this, &AChunkWorld::SpawnChunk, 0.05f, true);
				}
			}

			ChunkCount++;
		}
	}

	if (ChunkCount == FullChunkCount)
	{
		ChunkCount = 0;
	}
}

void AChunkWorld::Generate3DWorld()
{
	for (int x = -DrawDistance; x <= DrawDistance; x++)
	{
		for (int y = -DrawDistance; y <= DrawDistance; y++)
		{
			auto transform = FTransform(
				FRotator::ZeroRotator,
				FVector(x * Size * 100, y * Size * 100, 0),
				FVector::OneVector
			);

			if (CurrentChunks.Find(transform.GetLocation()))
			{
				ChunkCount++;
				break;
			}

			ChunkQueue.Enqueue(transform);

			if (!ChunkQueue.IsEmpty())
			{
				if (!GetWorldTimerManager().IsTimerActive(SpawnChunksHandle))
				{
					GetWorldTimerManager().SetTimer(SpawnChunksHandle, this, &AChunkWorld::SpawnChunk, 0.02f, true);
				}
			}
			ChunkCount++;
		}
	}

	if (ChunkCount == FullChunkCount)
	{
		ChunkCount = 0;
	}
}