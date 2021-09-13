#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATankPawn;

UCLASS()
class TANKOGEDDON_API ATankAIController : public AAIController
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    ATankPawn* TankPawn;

    UPROPERTY()
    APawn* PlayerPawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
    TArray<FVector> PatrollingPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
    float MovementAccuracy;

    int32 CurrentPatrolPointIndex = INDEX_NONE;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRange = 1000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingSpeed = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accuracy = 10;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    float GetRotationValue();
    void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange();
    bool DetectCanFire();
    void Fire();
    bool DetectPlayerVisibility();
};