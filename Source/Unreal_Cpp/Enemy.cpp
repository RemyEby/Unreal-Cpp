// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector Direction = PlayerLocation - GetActorLocation();
	FVector TargetLocation = GetActorLocation() + (Direction * DeltaTime * MovementSpeed);
	SetActorLocation(TargetLocation);
}

bool AEnemy::GetDamage(float damage)
{
	_life -= damage;

	if (_life <= 0)
	{
		Destroy();

		return true;
	}
	return false;
}
