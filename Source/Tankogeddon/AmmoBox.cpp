#include "AmmoBox.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "TankPawn.h"
#include <GameFramework/Controller.h>
#include <Engine/World.h>
#include "Cannon.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* playerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OtherActor == playerPawn)
	{
		ACannon* Cannon = playerPawn->GetActiveCannon();
		if (Cannon && Cannon->GetClass() == CannonClass)
		{
			Cannon->AddAmmo(NumAmmo);
		}
		else
		{
			playerPawn->SetupCannon(CannonClass);
		}
		Destroy();
	}
}
