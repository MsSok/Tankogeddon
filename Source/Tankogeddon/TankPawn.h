// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;
class UHealthComponent;
class UBoxComponent;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void RotateRight(float AxisValue);

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void FireSpecial();

	UFUNCTION()
	void SetupCannon(TSubclassOf<ACannon> InCannonClass);

	UFUNCTION()
	void CycleCannon();

	UFUNCTION()
	ACannon* GetActiveCannon() const;

	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float InterpolationKey = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Speed")
	float TurretRotationInterpolationKey = 0.5f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY()
	ATankPlayerController* TankController;
	
	UPROPERTY()
	ACannon* ActiveCannon;

	UPROPERTY()
	ACannon* InactiveCannon;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float _targetForwardAxisValue = 0.f;
	float _targetRightAxisValue = 0.f;
	float _targetRotateRightAxisValue = 0.f;
	float _currentRightAxisValue = 0.f;
};
