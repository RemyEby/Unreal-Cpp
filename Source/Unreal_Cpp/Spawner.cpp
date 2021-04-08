// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include <Unreal_Cpp/Enemy.h>

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	RemainingEnemies = EnemiesPerWave;

	SpawnEnemy();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASpawner::SpawnEnemy()
{

	GetWorld()->GetTimerManager().SetTimer(WaveTimer, [this]() {
		if (NumOfWaves > 0) {
			GetWorld()->GetTimerManager().SetTimer(EnemyTimer, [this]() {
				if (RemainingEnemies > 0)
				{
					FActorSpawnParameters SpawnInfo;
					AEnemy* const SpawnedActor = GetWorld()->SpawnActor<AEnemy>(ActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnInfo);

					RemainingEnemies--;
				}
				else
				{
					NumOfWaves--;
					RemainingEnemies = EnemiesPerWave;
					GetWorld()->GetTimerManager().ClearTimer(EnemyTimer);
				}
			}, TimeBetweenSpawn, 1);
		}
		else {
			GetWorld()->GetTimerManager().ClearTimer(WaveTimer);

		}
	}, TimeBetweenWave, 1);
}
