
#include "TankPawn.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathUtility.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>
#include "Tankogeddon.h"
#include "TankPlayerController.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include <Components/BoxComponent.h>
#include "Scorable.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPawn::TargetDestroyed(AActor* Target)
{
	if (IScorable* Scorable = Cast<IScorable>(Target))
	{
		AccumulatedScores += Scorable->GetScores();
		UE_LOG(LogTemp, Log, TEXT("Destroyed target %s. Current scores: %d"), *Target->GetName(), AccumulatedScores);
	}
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank movement
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector movePosition = currentLocation + forwardVector * MoveSpeed * _targetForwardAxisValue * DeltaTime;
	SetActorLocation(movePosition, true);

	// Tank rotation
	_currentRightAxisValue = FMath::Lerp(_currentRightAxisValue, _targetRightAxisValue, InterpolationKey);
	float yawRotation = RotationSpeed * _currentRightAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation = currentRotation.Yaw + yawRotation;
	FRotator newRotation = FRotator(0, yawRotation, 0);
	SetActorRotation(newRotation);
}

void ATankPawn::MoveForward(float AxisValue)
{
	_targetForwardAxisValue = AxisValue;
}

void ATankPawn::MoveRight(float AxisValue)
{
	_targetRightAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
	_targetRotateRightAxisValue = AxisValue;
}

void ATankPawn::DamageTaken(float InDamage)
{
	Super::DamageTaken(InDamage);

	if (this == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (HitForceEffect)
		{
			FForceFeedbackParameters HitForceEffectParams;
			HitForceEffectParams.bLooping = false;
			HitForceEffectParams.Tag = "HitForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(HitForceEffect, HitForceEffectParams);
		}

		if (HitShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitShake);
		}
	}
}
