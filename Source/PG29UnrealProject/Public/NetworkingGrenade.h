// Copyright VFS 2026 - Vi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkingGrenade.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class PG29UNREALPROJECT_API ANetworkingGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkingGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;


	UPROPERTY(EditDefaultsOnly, Category = "Grenade") 
	float FuseTime = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grenade") 
	float ExplosionRadius = 250.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grenade") 
	float ExplosionDamage = 80.0f;

	FTimerHandle FuseTimerHandle;

	void Explode();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
