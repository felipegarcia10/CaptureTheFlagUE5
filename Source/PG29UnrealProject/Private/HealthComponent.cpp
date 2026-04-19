// Copyright VFS 2026 - Rohit


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (AActor* OwningActor = GetOwner())
	{
		OwningActor->OnTakeAnyDamage.AddUniqueDynamic(this, &UHealthComponent::OnOwningActorTakenDamage);
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnRep_CurrentHealth(float InOldHealth)
{
	OnDamageReceived.Broadcast(InOldHealth, CurrentHealth, (InOldHealth - CurrentHealth));
}

void UHealthComponent::OnRep_MaxHealth(float InMaxHealth)
{
}

void UHealthComponent::OnOwningActorTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamagedActor && DamageCauser && InstigatedBy)
	{
		const float OldHealth = CurrentHealth;
		float NewHealth = FMath::Clamp<float>(CurrentHealth - Damage, 0.0f, MaxHealth);
		CurrentHealth = NewHealth;
		OnRep_CurrentHealth(OldHealth);
		if (CurrentHealth <= 0.0f)
		{
			DamagedActor->Destroy();
		}
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
}

