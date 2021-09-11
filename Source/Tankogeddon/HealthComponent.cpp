#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

bool UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float TakenDamage = DamageData.DamageValue;
	CurrentHealth -= TakenDamage;

	bool bWasDestroyed = false;
	if (CurrentHealth <= 0.f)
	{
		bWasDestroyed = true;
		if (OnDie.IsBound())
		{
			OnDie.Broadcast();
		}
	}
	else
	{
		if (OnDamaged.IsBound())
		{
			OnDamaged.Broadcast(TakenDamage);
		}
	}

	return bWasDestroyed;
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float AddiditionalHealthValue)
{
	CurrentHealth += AddiditionalHealthValue;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}