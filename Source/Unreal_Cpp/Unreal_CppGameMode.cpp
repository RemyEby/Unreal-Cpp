// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_CppGameMode.h"
#include "Unreal_CppHUD.h"
#include "Unreal_CppCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Unreal_Cpp/Enemy.h"
#include "Unreal_Cpp/Spawner.h"

AUnreal_CppGameMode::AUnreal_CppGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUnreal_CppHUD::StaticClass();
}

void AUnreal_CppGameMode::AddEnemyToList(AEnemy* Enemy)
{
	LAEnemies.Add(Enemy);
}

void AUnreal_CppGameMode::AddSpawner(ASpawner* Spawner)
{
	LASpawner.Add(Spawner);
}

void AUnreal_CppGameMode::DestroyEnemy(AEnemy* Enemy)
{
	if (USkeletalMeshComponent* Mesh = Enemy->FindComponentByClass<USkeletalMeshComponent>())
	{
		Mesh->PlayAnimation(Enemy->GetDeathAnim(), false);
		Enemy->SetMoveSpeed(0);
		Enemy->SetRotateSpeed(0);
	}

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this, Enemy]() {
		LAEnemies.Remove(Enemy);
		Enemy->Destroy();
	}, Enemy->GetDeathAnim()->SequenceLength, 0);
}

void AUnreal_CppGameMode::ReloadLevel()
{
	for (auto i = 0; i < LASpawner.Num(); i++)
	{
		LASpawner[i]->ResetSpawner();
	}

	for (auto i = 0; i < LAEnemies.Num(); i++)
	{
		LAEnemies[i]->Destroy();
	}
	LAEnemies.Empty();

	AUnreal_CppCharacter* Character = (AUnreal_CppCharacter*)GetWorld()->GetFirstPlayerController()->GetPawn();
	Character->ResetPlayer();

}