#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "Cannon.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class AProjectile;
class USceneComponent;
class AAmmoBox;
class UParticleSystemComponent;
class UAudioComponent;
class UForceFeedbackEffect;
class UMatineeCameraShake;

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	bool _HasSpecialFire = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int MaxAmmo = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UForceFeedbackEffect* ShootForceEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> ShootShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile;

	FTimerHandle ReloadTimerHandle;

	bool ReadyToFire = false;

public:
	// Sets default values for this actor's properties
	ACannon();
	void Fire();
	void FireSpecial();
	bool IsReadyToFire() const;
	bool HasSpecialFire() const;
	void SetVisibility(bool bIsVisible);
	void AddAmmo(int InNumAmmo);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestoyedTarget, AActor*);
	FOnDestoyedTarget OnDestroyedTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Reload();

	void NotifyTargetDestroyed(AActor* Target);

	int NumAmmo = 10;
};