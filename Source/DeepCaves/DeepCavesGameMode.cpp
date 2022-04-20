// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeepCavesGameMode.h"
#include "DeepCavesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeepCavesGameMode::ADeepCavesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
