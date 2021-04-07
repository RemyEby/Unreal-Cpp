// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include <Unreal_Cpp/Unreal_CppCharacter.h>

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

	// Calcul des location et rotation entre le player et l'actor
	FVector Direction = PlayerLocation - GetActorLocation();
	Direction.Normalize();
	FVector TargetLocation = GetActorLocation() + (Direction * DeltaTime * MovementSpeed);
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLocation);
	FRotator TargetRotation = FMath::RInterpTo(GetActorRotation(), Rotation, DeltaTime, RotationSpeed);

	// Application des nouvelles location et rotation à l'actor
	FVector NewLocation = FVector(TargetLocation.X, TargetLocation.Y, GetActorLocation().Z); // On ne veut pas modifier la location en Z 
	FRotator NewRotation = FRotator(0.0f, TargetRotation.Yaw - 90.0f, 0.0f); // On ajoute -90.f pour que l'actor soit bien en face de la direction dans laquelle il va.
	SetActorLocationAndRotation(NewLocation, NewRotation, false, 0, ETeleportType::None); // l'actor se dirige vers le player en le regardant toujours en face

	float Distance = FVector::Dist(PlayerLocation, GetActorLocation());
	UE_LOG(LogTemp, Log, TEXT("%f"), Distance);
	if (Distance <= 200 && !IsAttacking && !IsDying)
	{
		Attack();
	}
	
}

bool AEnemy::GetDamage(float damage)
{
	_life -= damage;

	if (_life <= 0 && !IsDying)
	{
		IsDying = true;

		return true;
	}
	return false;
}

void AEnemy::Attack()
{
	IsAttacking = true;

	if (USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>())
	{
		Mesh->PlayAnimation(AttackAnimation, false);
		SetMoveSpeed(0);
		SetRotateSpeed(0);

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this, Mesh]() {
			if (!IsDying)
			{
				Mesh->PlayAnimation(RunAnimation, true);
				SetMoveSpeed(InitialMovementSpeed);
				SetRotateSpeed(InitialRotationSpeed);
				IsAttacking = false;
			}
		}, AttackAnimation->SequenceLength, 0);
		
		AUnreal_CppCharacter* Player = (AUnreal_CppCharacter*)GetWorld()->GetFirstPlayerController()->GetPawn();
		Player->GetDamage(_damage);
	}
}