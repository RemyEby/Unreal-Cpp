// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Unreal_CppGameMode.generated.h"

class AEnemy;
class ASpawner;

UCLASS(minimalapi)
class AUnreal_CppGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TArray<AEnemy*> LAEnemies;

	TArray<ASpawner*> LASpawner;

public:
	AUnreal_CppGameMode();

	TArray<AEnemy*> GetListEnemy() { return LAEnemies; }
	TArray<ASpawner*> GetListSpawner() { return LASpawner; }
	void AddEnemyToList(AEnemy*);
	void AddSpawner(ASpawner*);

	void DestroyEnemy(AEnemy* Enemy);

	void ReloadLevel();
};



