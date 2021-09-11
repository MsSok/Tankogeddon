#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "DamageTaker.h"
#include "GameStructs.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	SetLifeSpan(FlyRange / MoveSpeed);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
	AActor* owner = GetOwner();
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

	bool bWasTargetDestroyed = false;
    if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
    {
        OtherActor->Destroy();
        bWasTargetDestroyed = true;
    }
    else if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(OtherActor))
    {
        AActor* MyInstigator = GetInstigator();
        if (OtherActor != MyInstigator)
        {
            FDamageData DamageData;
            DamageData.DamageValue = Damage;
            DamageData.DamageMaker = this;
            DamageData.Instigator = MyInstigator;
            DamageTaker->TakeDamage(DamageData);
            bWasTargetDestroyed = DamageTaker->TakeDamage(DamageData);
        }
    }

    if (bWasTargetDestroyed && OnDestroyedTarget.IsBound())
    {
        OnDestroyedTarget.Broadcast(OtherActor);
    }
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}