// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelFunctionLibrary.h"

FIntVector UVoxelFunctionLibrary::WorldToBlockPosition(const FVector& Position)
{
	return FIntVector(Position) / 100;
}

FIntVector UVoxelFunctionLibrary::WorldToLocalBlockPosition(const FVector& Position, const int Size)
{
	const auto ChunkPos = WorldToChunkPosition(Position, Size);

	auto Result = WorldToBlockPosition(Position) - ChunkPos * Size;

	// Negative Normalization
	if (ChunkPos.X < 0) Result.X--;
	if (ChunkPos.Y < 0) Result.Y--;
	if (ChunkPos.Z < 0) Result.Z--;

	return Result;
}

FIntVector UVoxelFunctionLibrary::WorldToChunkPosition(const FVector& Position, const int Size)
{
	FIntVector Result;

	const int Factor = Size * 100;
	const auto IntPosition = FIntVector(Position);

	if (IntPosition.X < 0) Result.X = (int)(Position.X / Factor) - 1;
	else Result.X = (int)(Position.X / Factor);

	if (IntPosition.Y < 0) Result.Y = (int)(Position.Y / Factor) - 1;
	else Result.Y = (int)(Position.Y / Factor);

	if (IntPosition.Z < 0) Result.Z = (int)(Position.Z / (Factor * 24)) - 1;
	else Result.Z = (int)(Position.Z / (Factor * 24));

	return Result;
}

FVector UVoxelFunctionLibrary::FloorVector(const FVector& Vector)
{
	return FVector(FMath::Floor(Vector.X), FMath::Floor(Vector.Y), FMath::Floor(Vector.Z));
}

FVector UVoxelFunctionLibrary::CeilVector(const FVector& Vector)
{
	return FVector(FMath::CeilToInt(Vector.X), FMath::CeilToInt(Vector.Y), FMath::CeilToInt(Vector.Z));
}