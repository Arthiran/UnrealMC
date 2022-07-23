// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkVector.generated.h"

USTRUCT(BlueprintType)
struct UNREALMC_API FChunkVector
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		FVector ChunkPosition = FVector::ZeroVector;
    
    FChunkVector()
    {}

    FChunkVector(FVector InChunkPosition)
        : ChunkPosition(InChunkPosition)
    {}

    FChunkVector(const FChunkVector& Other)
        : FChunkVector(Other.ChunkPosition)
    {}

    bool operator==(const FChunkVector& Other) const
    {
        return Equals(Other);
    }

    bool Equals(const FChunkVector& Other) const
    {
        return ChunkPosition.Equals(Other.ChunkPosition);
    }
};
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FHashMeIfYouCan& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FChunkVector& Thing)
{
    uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FChunkVector));
    return Hash;
}
#endif
