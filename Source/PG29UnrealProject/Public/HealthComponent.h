// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


class UDamageType;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageReceived, const float, Damage, const float, OldHealth, const float, NewHealth);

UCLASS( ClassGroup=(NetworkingComponents), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class PG29UNREALPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_CurrentHealth, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_MaxHealth, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	float MaxHealth;

	UFUNCTION()
	void OnRep_CurrentHealth(float InOldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(float InMaxHealth);

	UFUNCTION()
	void OnOwningActorTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnDamageReceived OnDamageReceived;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		
};
