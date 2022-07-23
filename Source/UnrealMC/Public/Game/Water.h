// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/ChunkMeshData.h"
#include "Utils/Enums.h"
#include "Water.generated.h"

class UProceduralMeshComponent;

UCLASS()
class UNREALMC_API AWater : public AActor
{
	GENERATED_BODY()

	struct FMask
	{
		EBlock Block;
		int Normal;
	};
	
public:	
	// Sets default values for this actor's properties
	AWater();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Chunk")
		TObjectPtr<UMaterialInterface> Material;

	int Size = 16;
	TArray<EBlock> MainBlocks;

	void ModifyVoxel(const FIntVector Position);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateWater();
	void GenerateMesh();

	void ModifyVoxelData(const FIntVector Position);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Water")
		UProceduralMeshComponent* WaterMesh;
	FChunkMeshData WaterMeshData;
	int WaterVertexCount = 0;
	TArray<EBlock> WaterBlocks;

private:
	void Setup();

	void CreateWaterQuad(FMask Mask, FIntVector AxisMask, int Width, int Height, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);
	EBlock GetWaterBlock(FIntVector Index) const;
	//EBlock GetCompareWaterBlock(FIntVector OriginalIndex, FIntVector Index) const;
	int GetWaterBlockIndex(int X, int Y, int Z) const;

	void ApplyMesh() const;
	void ClearMesh();

	bool CompareMask(FMask M1, FMask M2) const;

};
