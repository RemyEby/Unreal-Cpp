// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AEnemy;

UCLASS()
class UNREAL_CPP_API ASpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float TimeBetweenSpawn = 2.5f;

	UPROPERTY(EditAnywhere)
	int EnemiesToSpawn = 5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> ActorToSpawn;
	
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

};
