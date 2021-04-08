// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Unreal_Cpp/Unreal_CppCharacter.h"
#include "Unreal_Cpp/Enemy.h"
#include "Unreal_Cpp/Unreal_CppGameMode.h"
#include "Camera/CameraComponent.h"


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
	
	_ammo = _maxAmmo;
	SKMesh = FindComponentByClass<USkeletalMeshComponent>();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnFire(UCameraComponent* FirstPersonCameraComponent, UParticleSystem* pMuzzleParticle, UParticleSystem* pImpactParticle, UAnimMontage* FireAnimation, USkeletalMeshComponent* Mesh1P)
{
	if (!_isReloading)
		{
			if (_ammo > 0)
			{
				_ammo--;
	
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%d /30 ammo"), _ammo));
				UE_LOG(LogTemp, Warning, TEXT("%d"), _ammo);
	
				// try and fire a projectile
				auto StartLocation = FirstPersonCameraComponent->GetComponentLocation();
				auto EndLocation = StartLocation + FirstPersonCameraComponent->GetForwardVector() * 100000.f;
	
				FHitResult OutHit;
				FCollisionQueryParams Collisions;
	
				if (GetWorld()->UWorld::LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility, Collisions)) {
					if (pMuzzleParticle) {
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pMuzzleParticle, SKMesh->GetSocketTransform(FName("Muzzle")));
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pImpactParticle, OutHit.ImpactPoint);
	
						if (OutHit.GetActor()->GetName().Contains("Enemy_BP"))
						{
							AEnemy* enemy = (AEnemy*)OutHit.GetActor();
							if (enemy->GetDamage(_damage))
							{
								AUnreal_CppGameMode* GameMode = (AUnreal_CppGameMode*)GetWorld()->GetAuthGameMode();
								GameMode->DestroyEnemy(enemy);
							}
						}
					}
				}
	
				// try and play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}
	
				// try and play a firing animation if specified
				if (FireAnimation != nullptr)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}
				}
			}
			else {
				// Attendre quelques secondes que ce soit rechargé
				Reloading();
			}
		}
}

void AWeapon::Reloading()
{
	if (!_isReloading)
	{
		_isReloading = true;
		_ammo = 0;
		UE_LOG(LogTemp, Warning, TEXT("RELOADING ..."));
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
			_ammo = _maxAmmo;
			_isReloading = false;
		}, 2.5f, 0);
	}
}