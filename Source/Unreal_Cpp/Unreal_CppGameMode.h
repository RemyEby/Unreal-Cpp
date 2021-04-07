// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Unreal_CppGameMode.generated.h"

class AEnemy;

UCLASS(minimalapi)
class AUnreal_CppGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnreal_CppGameMode();

	void DestroyEnemy(AEnemy* Enemy);
};



