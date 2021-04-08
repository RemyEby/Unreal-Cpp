// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Unreal_Cpp/Unreal_CppCharacter.h"
#include "Unreal_Cpp/Enemy.h"
#include "Unreal_Cpp/Unreal_CppGameMode.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnFire() {}

void AWeapon::Reloading()
{
	if (!_isReloading)
	{
		_isReloading = true;
		_ammo = 0;
		UE_LOG(LogTemp, Warning, TEXT("RELOADING ..."), _ammo);
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
			_ammo = _maxAmmo;
			_isReloading = false;
		}, 2.5f, 0);
	}
}