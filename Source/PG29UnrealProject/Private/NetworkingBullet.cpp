// Copyright VFS 2026 - Rohit


#include "NetworkingBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ANetworkingBullet::ANetworkingBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	// Collission
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionComponent->InitSphereRadius(32.0f);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	SetRootComponent(CollisionComponent);

	// Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));;
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Projectile Movement Component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.1f;

	// Lifetime
	SetLifeSpan(10.0f);
}

// Called when the game starts or when spawned
void ANetworkingBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetworkingBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

