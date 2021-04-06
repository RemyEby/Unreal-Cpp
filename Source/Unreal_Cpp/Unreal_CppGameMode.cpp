// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_CppGameMode.h"
#include "Unreal_CppHUD.h"
#include "Unreal_CppCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnreal_CppGameMode::AUnreal_CppGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUnreal_CppHUD::StaticClass();
}
