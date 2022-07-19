// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealMCGameMode.h"
#include "UnrealMCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealMCGameMode::AUnrealMCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
