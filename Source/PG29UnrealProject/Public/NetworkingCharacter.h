// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NetworkingCharacter.generated.h"


class ANetworkingBullet;
class ANetworkingGrenade;
class UHealthComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PG29UNREALPROJECT_API ANetworkingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANetworkingCharacter();

	UFUNCTION(Client, Reliable)
	void OnPossessed_Client(AController* NewController);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Shoot_Server();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ThrowGrenade_Server();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

	UFUNCTION(BlueprintNativeEvent)
	FTransform GetBulletSpawnTransform() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting")
	TSubclassOf<ANetworkingBullet> BulletClassToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grenade")
	TSubclassOf<ANetworkingGrenade> GrenadeClassToSpawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	const TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MovementInputAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireSingleInputAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ThrowGrenadeInputAction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void OnMovementInputReceived(const FInputActionValue& Value);
	void OnLookInputReceived(const FInputActionValue& Value);
	void OnJumpInputReceived(const FInputActionValue& Value);
	void OnFireInputReceived(const FInputActionValue& Value);
	void OnThrowGrenadeInputRecieved(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
