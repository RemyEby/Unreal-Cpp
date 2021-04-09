// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include <Unreal_Cpp/Enemy.h>
#include "Unreal_CppGameMode.h"

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

	GameMode = (AUnreal_CppGameMode*)GetWorld()->GetAuthGameMode();
	GameMode->AddSpawner(this);

	

	RemainingEnemies = EnemiesPerWave;
	RemainingWaves = NumOfWaves;

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
		if (RemainingWaves > 0) {
			GetWorld()->GetTimerManager().SetTimer(EnemyTimer, [this]() {
				if (RemainingEnemies > 0)
				{
					FActorSpawnParameters SpawnInfo;
					AEnemy* SpawnedActor = GetWorld()->SpawnActor<AEnemy>(ActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnInfo);
					GameMode->AddEnemyToList(SpawnedActor);

					RemainingEnemies--;
				}
				else
				{
					RemainingWaves--;
					RemainingEnemies = EnemiesPerWave;
					GetWorld()->GetTimerManager().ClearTimer(EnemyTimer);
				}
			}, TimeBetweenSpawn, 1);
		}
		else {
			GetWorld()->GetTimerManager().ClearTimer(WaveTimer);		}
	}, TimeBetweenWave, 1);
}

void ASpawner::ResetSpawner()
{
	UE_LOG(LogTemp, Log, TEXT("Reset level"));
	GetWorld()->GetTimerManager().ClearTimer(EnemyTimer);
	GetWorld()->GetTimerManager().ClearTimer(WaveTimer);
	RemainingEnemies = EnemiesPerWave;
	RemainingWaves = NumOfWaves;

	SpawnEnemy();
}