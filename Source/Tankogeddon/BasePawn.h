#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStructs.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "BasePawn.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class ACannon;
class UHealthComponent;
class UBoxComponent;

UCLASS()
class TANKOGEDDON_API ABasePawn : public APawn, public IDamageTaker
{
    GENERATED_BODY()

public:
    ABasePawn();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* BodyMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UArrowComponent* CannonSetupPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* HitCollider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Cannon")
    TSubclassOf<ACannon> CannonClass;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void Destroyed() override;
    virtual void TargetDestroyed(AActor* Target);

public:
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
    virtual bool TakeDamage(FDamageData DamageData) override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    ACannon* ActiveCannon;

    UPROPERTY()
    ACannon* InactiveCannon;

    UFUNCTION()
    void Die();

    UFUNCTION()
    void DamageTaken(float InDamage);
};