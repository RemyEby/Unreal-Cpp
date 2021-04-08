// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class UNREAL_CPP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float _damage = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float _fireRate = .2f;

	UPROPERTY(EditDefaultsOnly)
	int _maxAmmo = 30;
	int _ammo = _maxAmmo;

	UPROPERTY(EditDefaultsOnly)
	float _reloadTime = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly)
	bool _AutomaticWeapon;

	bool _shoot = false;
	bool _isReloading = false;

public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetDamage() { return _damage; }
	float GetFireRate() { return _fireRate; }
	int GetMaxAmmo() { return _ammo; }
	float GetReloadTime() { return _reloadTime; }
	
	bool GetBoolShoot() { return _shoot; }
	void SetBoolShoot(bool b) { _shoot = b; }
	bool GetBoolIsReloading() { return _isReloading; }
	void SetBoolIsReloading(bool b) { _isReloading = b; }

	void OnFire();
	void Reloading();
};
