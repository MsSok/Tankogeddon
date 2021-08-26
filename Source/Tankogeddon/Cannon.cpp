// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Tankogeddon.h"

ACannon::ACannon() {
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire() {
	if (!ReadyToFire || Ammu <= 0) {
		return;
	}
	ReadyToFire = false;
	--Ammu;

	if (Type == ECannonType::FireProjectile) {
		//GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire - projectile!"));
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, FString::Printf(TEXT("Fire! Ammo: %d"), Ammu));
	} else {
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}


void ACannon::FireSpecial() {
	if (!_HasSpecialFire || !ReadyToFire || Ammu <= 0)
	{
		return;
	}

	//ReadyToFire = false;
	_HasSpecialFire = false;
	--Ammu;

	if (Type == ECannonType::FireProjectile) {
		//GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire special - projectile!"));
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, FString::Printf(TEXT("SpecialFire! Ammo: %d"), Ammu));
	} else {
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire special - trace");
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}

bool ACannon::IsReadyToFire() const {
	return ReadyToFire;
}

bool ACannon::HasSpecialFire() const {
	return _HasSpecialFire;
}

void ACannon::Reload()
{
	ReadyToFire = true;
	_HasSpecialFire = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}