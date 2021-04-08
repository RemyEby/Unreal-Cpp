// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class USkeletalMeshComponent;

UCLASS()
class UNREAL_CPP_API AEnemy : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MeshEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UAnimSequence* RunAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UAnimSequence* DeathAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UAnimSequence* AttackAnimation;

	FVector PlayerLocation;

	UPROPERTY(EditDefaultsOnly)
	float InitialMovementSpeed = 300.f;
	float MovementSpeed = InitialMovementSpeed;

	UPROPERTY(EditDefaultsOnly)
	float InitialRotationSpeed = 200.f;
	float RotationSpeed = InitialRotationSpeed;

	UPROPERTY(EditDefaultsOnly)
	float _life = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float _damage = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	float _attackRange = 250.f;

	bool IsDying = false;
	bool IsAttacking = false;

public:
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetDamage(float damage);

	void SetMoveSpeed(float NewMSpeed) { MovementSpeed = NewMSpeed; }
	void SetRotateSpeed(float NewRSpeed) { RotationSpeed = NewRSpeed; }

	UAnimSequence* GetDeathAnim() { return DeathAnimation; }

	void Attack();

};
