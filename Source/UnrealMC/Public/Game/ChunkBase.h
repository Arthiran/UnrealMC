// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/ChunkMeshData.h"
#include "Utils/Enums.h"
#include "GameFramework/Actor.h"
#include "ChunkBase.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;
class FHeightmapGenerator;
class AWater;
class AObjectBase;

UCLASS(Abstract)
class UNREALMC_API AChunkBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChunkBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Chunk")
		int Size = 16;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Chunk")
		TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditInstanceOnly, Category = "Height Map")
		float Frequency;

	UPROPERTY(EditInstanceOnly, Category = "Height Map")
		EGenerationType GenerationType;

	UPROPERTY(EditDefaultsOnly, Category = Curve)
		UCurveFloat* Continentalness;

	FVector RandomOffset;

	UPROPERTY(EditDefaultsOnly, Category = "World")
		TSubclassOf<AWater> WaterType;
	UPROPERTY(EditDefaultsOnly, Category = "World")
		TSubclassOf<AObjectBase> FoliageType;

	UFUNCTION(BlueprintCallable, Category = "Chunk")
		EBlock ModifyVoxel(const FIntVector Position, const EBlock Block);

	UFUNCTION(BlueprintCallable, Category = "Chunk")
		void SelfDestruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Setup() PURE_VIRTUAL(AChunkBase::Setup);
	virtual void Generate2DHeightMap(const FVector Position) PURE_VIRTUAL(AChunkBase::Generate2DHeightMap);
	virtual void Generate3DHeightMap(const FVector Position) PURE_VIRTUAL(AChunkBase::Generate3DHeightMap);
	virtual void GenerateMesh() PURE_VIRTUAL(AChunkBase::GenerateMesh);

	virtual void ModifyVoxelData(const FIntVector Position, const EBlock Block) PURE_VIRTUAL(AChunkBase::ModifyVoxelData);

	FastNoiseLite* Noise;

	// Main Mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		UProceduralMeshComponent* Mesh;
	FChunkMeshData MeshData;
	int VertexCount = 0;
	TArray<EBlock> Blocks;
	FHeightmapGenerator* Worker;

	bool FinishedHMGeneration = false;

	AWater* Water;

	bool bIsHidden = false;

private:
	void ClearMesh();
	void CreateFoliage();
	void CreateWater();
	void GenerateHeightMap();
	void ApplyMesh() const;

	int GetBlockIndex(int X, int Y, int Z) const;
};
