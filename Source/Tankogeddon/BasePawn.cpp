#include "BasePawn.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include "HealthComponent.h"
#include "Tankogeddon.h"
#include "Cannon.h"

ABasePawn::ABasePawn()
{
    PrimaryActorTick.bCanEverTick = false;

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
    BodyMesh->bEditableWhenInherited = true;
    RootComponent = BodyMesh;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
    TurretMesh->SetupAttachment(BodyMesh);
    TurretMesh->bEditableWhenInherited = true;

    CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
    CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);
    CannonSetupPoint->bEditableWhenInherited = true;

    HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
    HitCollider->SetupAttachment(BodyMesh);
    HitCollider->bEditableWhenInherited = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
    HealthComponent->OnDie.AddDynamic(this, &ABasePawn::Die);
    HealthComponent->OnDamaged.AddDynamic(this, &ABasePawn::DamageTaken);
    HealthComponent->bEditableWhenInherited = true;
}

void ABasePawn::BeginPlay()
{
    Super::BeginPlay();

    SetupCannon(CannonClass);
}

void ABasePawn::Destroyed()
{
    if (ActiveCannon)
    {
        ActiveCannon->Destroy();
    }

    if (InactiveCannon)
    {
        InactiveCannon->Destroy();
    }
}

void ABasePawn::TargetDestroyed(AActor* Target)
{

}

void ABasePawn::Fire()
{
    if (ActiveCannon)
    {
        ActiveCannon->Fire();
    }
}

void ABasePawn::FireSpecial()
{
    if (ActiveCannon)
    {
        ActiveCannon->FireSpecial();
    }
}

void ABasePawn::SetupCannon(TSubclassOf<ACannon> InCannonClass)
{
    if (ActiveCannon)
    {
        ActiveCannon->Destroy();
        ActiveCannon = nullptr;
    }

    FActorSpawnParameters Params;
    Params.Instigator = this;
    Params.Owner = this;
    ActiveCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Params);
    ActiveCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    ActiveCannon->OnDestroyedTarget.AddUObject(this, &ABasePawn::TargetDestroyed);
}

void ABasePawn::CycleCannon()
{
    Swap(ActiveCannon, InactiveCannon);
    if (ActiveCannon)
    {
        ActiveCannon->SetVisibility(true);
    }

    if (InactiveCannon)
    {
        InactiveCannon->SetVisibility(false);
    }
}

ACannon* ABasePawn::GetActiveCannon() const
{
    return ActiveCannon;
}

bool ABasePawn::TakeDamage(FDamageData DamageData)
{
    return HealthComponent->TakeDamage(DamageData);
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABasePawn::Die()
{
    Destroy();
}

void ABasePawn::DamageTaken(float InDamage)
{
    UE_LOG(LogTemp, Log, TEXT("Pawn %s taken damage:%f "), *GetName(), InDamage);
}