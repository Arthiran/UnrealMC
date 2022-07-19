// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/Enums.h"
#include "ChunkVector.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

class AChunkBase;
class FastNoiseLite;

UCLASS()
class AChunkWorld : public AActor
{
	GENERATED_BODY()

	AChunkWorld();

public:
	UPROPERTY(EditInstanceOnly, Category = "World")
		TSubclassOf<AChunkBase> ChunkType;

	UPROPERTY(EditInstanceOnly, Category = "World")
		int DrawDistance = 5;

	UPROPERTY(EditInstanceOnly, Category = "Chunk")
		TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Chunk")
		int Size = 16;

	UPROPERTY(EditInstanceOnly, Category = "Height Map")
		EGenerationType GenerationType;

	UPROPERTY(EditInstanceOnly, Category = "Height Map")
		float Frequency = 0.03f;

	UPROPERTY(EditDefaultsOnly, Category = Curve)
		UCurveFloat* Continentalness;

	FVector RandomOffset;

	UFUNCTION(BlueprintCallable, Category = "Chunk")
		EBlock FindAndModifyChunk(const FIntVector BlockPosition, const FIntVector LocalPosition, const EBlock Block);

	UFUNCTION(BlueprintCallable, Category = "Chunk")
		void UpdateChunks(const FIntVector PlayerPosition);

	UPROPERTY(BlueprintReadOnly, Category = "Chunk")
		bool bIsStarted = false;

	TMap<FChunkVector, AChunkBase*> CurrentChunks = TMap<FChunkVector, AChunkBase*>();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnChunk();
	void RemoveChunks(const FIntVector PlayerPosition);

	FTimerHandle SpawnChunksHandle;

	int FullChunkCount = 0;
private:
	FIntVector CurrentPlayerPosition = FIntVector(0,0,0);
	int ChunkCount = 0;

	TQueue<FTransform> ChunkQueue = TQueue<FTransform>();

	void Generate3DWorld();
};