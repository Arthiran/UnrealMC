// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Enums.h"
#include "Utils/ChunkMeshData.h"
#include "BlockDefinition.h"
#include "ObjectBase.generated.h"

class UProceduralMeshComponent;

UCLASS()
class UNREALMC_API AObjectBase : public AActor
{
	GENERATED_BODY()
	
	struct FMask
	{
		EBlock Block;
		int Normal;
	};

public:	
	AObjectBase();

	void ModifyVoxel(const FIntVector Position, const EBlock Block);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Chunk")
		TObjectPtr<UMaterialInterface> Material;
	int Size = 5;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual TArray<FBlockDefinition> ObjectDefinition(const FIntVector Offset);

	void GenerateObject();
	void GenerateMesh();

	void ModifyVoxelData(const FIntVector Position, const EBlock Block);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Water")
		UProceduralMeshComponent* ObjectMesh;
	FChunkMeshData ObjectMeshData;
	int ObjectVertexCount = 0;
	TArray<EBlock> ObjectBlocks;

private:
	void Setup();

	void CreateObjectQuad(FMask Mask, FIntVector AxisMask, int Width, int Height, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);
	EBlock GetObjectBlock(FIntVector Index) const;
	//EBlock GetCompareWaterBlock(FIntVector OriginalIndex, FIntVector Index) const;
	int GetObjectBlockIndex(int X, int Y, int Z) const;

	void ApplyMesh() const;
	void ClearMesh();

	bool CompareMask(FMask M1, FMask M2) const;

};
