// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UCameraComponent;
class UAnimMontage;
class USkeletalMeshComponent;

UCLASS()
class UNREAL_CPP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int _weaponID = 0;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float _damage = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float _fireRate = .2f;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int _maxAmmo = 30;
	int _ammo;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float _reloadTime = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
	bool _AutomaticWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	USoundBase* ReloadSound;

	bool _isReloading = false;

	USkeletalMeshComponent* SKMesh;

public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetWeaponID() { return _weaponID; }
	float GetDamage() { return _damage; }
	float GetFireRate() { return _fireRate; }
	int GetMaxAmmo() { return _ammo; }
	float GetReloadTime() { return _reloadTime; }
	USkeletalMeshComponent* GetSKMesh() { return SKMesh; }
	bool GetIsAutomatic() { return _AutomaticWeapon; }
	
	bool GetBoolIsReloading() { return _isReloading; }
	void SetBoolIsReloading(bool b) { _isReloading = b; }

	void OnFire(UCameraComponent* , UParticleSystem* , UParticleSystem* , UAnimMontage* , USkeletalMeshComponent*);
	void Reloading();
};
