// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AEnemy;
class AUnreal_CppGameMode;

UCLASS()
class UNREAL_CPP_API ASpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float TimeBetweenSpawn = 1.f;

	UPROPERTY(EditAnywhere)
	float TimeBetweenWave = 10.f;

	UPROPERTY(EditAnywhere)
	int EnemiesPerWave = 5;
	int RemainingEnemies;

	UPROPERTY(EditAnywhere)
	int NumOfWaves = 3;
	int RemainingWaves;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> ActorToSpawn;

	FTimerHandle WaveTimer;

	FTimerHandle EnemyTimer;

	AUnreal_CppGameMode* GameMode;
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();
	void ResetSpawner();
};
