// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_CppCharacter.h"
#include "Unreal_CppProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Enemy.h"
#include <Unreal_Cpp/Unreal_CppGameMode.h>
#include "Weapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AUnreal_CppCharacter

AUnreal_CppCharacter::AUnreal_CppCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AUnreal_CppCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	_InitialLocation = GetActorLocation();
	_InitialRotation = GetActorRotation();
	UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), _InitialRotation.Roll, _InitialRotation.Pitch, _InitialRotation.Yaw);
	_ammo = _maxAmmo;
	_timer = _rifleCooldwn;
	_life = _maxLife;

	for (auto i = 0; i < LAWeapons.Num(); i++)
	{
		AWeapon* tempWeapon = GetWorld()->SpawnActor<AWeapon>(LAWeapons[i]);
		tempWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		if (i != 0) { tempWeapon->GetSKMesh()->SetHiddenInGame(true); }
		TAWeapon.Add(tempWeapon);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnreal_CppCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind weapon event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUnreal_CppCharacter::Shoot);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AUnreal_CppCharacter::ShootToFalse);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AUnreal_CppCharacter::Reload);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AUnreal_CppCharacter::SwitchWeapon);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnreal_CppCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AUnreal_CppCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnreal_CppCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnreal_CppCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnreal_CppCharacter::LookUpAtRate);
}

void AUnreal_CppCharacter::Shoot()
{
	if (TAWeapon[iCurrentWeapon]->GetIsAutomatic())
	{
		_shoot = true;
	}
	else
	{
		if (_timer >= TAWeapon[iCurrentWeapon]->GetFireRate())
		{
			TAWeapon[iCurrentWeapon]->OnFire(FirstPersonCameraComponent, pMuzzleParticle, pImpactParticle, FireAnimation, Mesh1P);
			_timer = 0;
		}
	}
}

void AUnreal_CppCharacter::ShootToFalse()
{
	_shoot = false;
}

void AUnreal_CppCharacter::Reload()
{
	TAWeapon[iCurrentWeapon]->Reloading();
}

void AUnreal_CppCharacter::SwitchWeapon()
{
	iCurrentWeapon++;

	if (iCurrentWeapon % TAWeapon.Num() == 0)
		iCurrentWeapon = 0;

	for (auto i = 0; i < TAWeapon.Num(); i++)
	{
		if (i != iCurrentWeapon)
		{
			TAWeapon[i]->GetSKMesh()->SetHiddenInGame(true);
		}
		else
		{
			TAWeapon[iCurrentWeapon]->GetSKMesh()->SetHiddenInGame(false);
		}
	}
}

bool AUnreal_CppCharacter::GetDamage(float damage)
{
	_life -= damage;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"), _life));

	if (_life <= 0)
	{
		AUnreal_CppGameMode* GameMode = (AUnreal_CppGameMode*)GetWorld()->GetAuthGameMode();
		GameMode->ReloadLevel();

		return true;
	}
	return false;
}

void AUnreal_CppCharacter::ResetPlayer()
{
	SetActorLocation(_InitialLocation);
	_life = _maxLife;
	for (auto i = 0; i < TAWeapon.Num(); i++)
	{
		TAWeapon[i]->Reloading();
	}
}

// Called every frame
void AUnreal_CppCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_timer += DeltaTime;
	if (_shoot) // Si le joueur tire
	{
		if (_timer >= TAWeapon[iCurrentWeapon]->GetFireRate()) 
		{
			TAWeapon[iCurrentWeapon]->OnFire(FirstPersonCameraComponent, pMuzzleParticle, pImpactParticle, FireAnimation, Mesh1P);
			_timer = 0;
		}
	}
}

void AUnreal_CppCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnreal_CppCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	/*if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;*/
}

void AUnreal_CppCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	/*if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;*/
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AUnreal_CppCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AUnreal_CppCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUnreal_CppCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUnreal_CppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnreal_CppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AUnreal_CppCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUnreal_CppCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AUnreal_CppCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AUnreal_CppCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
