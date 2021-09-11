// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Tankogeddon.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "AmmoBox.h"

ACannon::ACannon() {
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire() {
	if (!ReadyToFire || NumAmmo <= 0) {
		return;
	}
	ReadyToFire = false;
	--NumAmmo;

	if (Type == ECannonType::FireProjectile) {
		//GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire - projectile!"));
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, FString::Printf(TEXT("Fire! Ammo: %d"), NumAmmo));
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
		{
			projectile->SetInstigator(GetInstigator());
			projectile->Start();
		}
	} else {
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;

		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (hitResult.Actor.Get())
			{
				hitResult.Actor.Get()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}


void ACannon::FireSpecial() {
	if (!_HasSpecialFire || !ReadyToFire || NumAmmo <= 0)
	{
		return;
	}

	//ReadyToFire = false;
	_HasSpecialFire = false;
	--NumAmmo;

	if (Type == ECannonType::FireProjectile) {
		//GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire special - projectile!"));
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, FString::Printf(TEXT("SpecialFire! Ammo: %d"), NumAmmo));
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
		{
			projectile->SetInstigator(GetInstigator());
			projectile->Start();
		}
	} else {
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire special - trace");
		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;

		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Yellow, false, 0.5f, 0, 5);
			if (hitResult.Actor.Get())
			{
				hitResult.Actor.Get()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Yellow, false, 0.5f, 0, 5);
		}
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

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::AddAmmo(int InNumAmmo)
{
	NumAmmo = FMath::Clamp(NumAmmo + InNumAmmo, 0, MaxAmmo);
	UE_LOG(LogTemp, Log, TEXT("AddAmmo(%d)! NumAmmo: %d"), InNumAmmo, NumAmmo);
}