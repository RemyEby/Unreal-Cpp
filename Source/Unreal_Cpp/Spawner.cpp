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

	SpawnEnemy();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ASpawner::SpawnEnemy()
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		FActorSpawnParameters SpawnInfo;
		AEnemy* const SpawnedActor = GetWorld()->SpawnActor<AEnemy>(ActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnInfo);
		
	}, TimeBetweenSpawn, 1);
}