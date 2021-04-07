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
	UAnimSequence* DeathAnimation;

	FVector PlayerLocation;

	UPROPERTY(EditDefaultsOnly)
		float MovementSpeed = .5f;

	UPROPERTY(EditDefaultsOnly)
		float RotationSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly)
		float _life = 1.0f;

	bool IsDying = false;

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

};
