// Copyright VFS 2026 - Rohit


#include "NetworkingCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Engine.h"
#include "NetworkingBullet.h"
#include "EnhancedInputComponent.h"
#include "HealthComponent.h"
#include "NetworkingGrenade.h"

// Sets default values
ANetworkingCharacter::ANetworkingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

FTransform ANetworkingCharacter::GetBulletSpawnTransform_Implementation() const
{
	return FTransform::Identity;
}

void ANetworkingCharacter::Shoot_Server_Implementation()
{
	if (UWorld* World = GetWorld())
	{
		if (BulletClassToSpawn)
		{
			const FTransform BulletSpawnTransform = GetBulletSpawnTransform();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = this;
			SpawnParams.Owner = GetPlayerState();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			World->SpawnActor<ANetworkingBullet>(BulletClassToSpawn, BulletSpawnTransform, SpawnParams);
		}
	}
}

void ANetworkingCharacter::OnPossessed_Client_Implementation(AController* NewController)
{
	if (APlayerController* NewPlayerController = Cast<APlayerController>(NewController))
	{
		if (ULocalPlayer* LocalPlayer = NewPlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called when the game starts or when spawned
void ANetworkingCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANetworkingCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	OnPossessed_Client(NewController);
}

// Called every frame
void ANetworkingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANetworkingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			if (MovementInputAction)
			{
				EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ANetworkingCharacter::OnMovementInputReceived);
			}

			if (LookInputAction)
			{
				EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ANetworkingCharacter::OnLookInputReceived);
			}

			if (JumpInputAction)
			{
				EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ANetworkingCharacter::OnJumpInputReceived);
			}

			if (FireSingleInputAction)
			{
				EnhancedInputComponent->BindAction(FireSingleInputAction, ETriggerEvent::Triggered, this, &ANetworkingCharacter::OnFireInputReceived);
			}

			if (ThrowGrenadeInputAction)
			{
				EnhancedInputComponent->BindAction(ThrowGrenadeInputAction, ETriggerEvent::Triggered, this, &ANetworkingCharacter::OnThrowGrenadeInputRecieved);
			}
		}
	}
}

void ANetworkingCharacter::OnMovementInputReceived(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FVector ForwardVector = GetActorForwardVector();
		const FVector RightVector = GetActorRightVector();
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void ANetworkingCharacter::OnLookInputReceived(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANetworkingCharacter::OnJumpInputReceived(const FInputActionValue& Value)
{
	Jump();
}

void ANetworkingCharacter::OnFireInputReceived(const FInputActionValue& Value)
{
	Shoot_Server();
}

void ANetworkingCharacter::OnThrowGrenadeInputRecieved(const FInputActionValue& Value)
{
	ThrowGrenade_Server();
}

void ANetworkingCharacter::ThrowGrenade_Server_Implementation()
{
	if (GrenadeClassToSpawn && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		SpawnParams.Owner = GetPlayerState();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ANetworkingGrenade>(GrenadeClassToSpawn, GetBulletSpawnTransform(), SpawnParams);
	}
}
