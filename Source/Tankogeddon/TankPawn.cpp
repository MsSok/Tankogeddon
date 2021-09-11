
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
	TankController = Cast<ATankPlayerController>(GetController());
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

	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = currentLocation + (forwardVector * _targetForwardAxisValue + rightVector * _targetRightAxisValue) * MoveSpeed * DeltaTime;

	SetActorLocation(movePosition, true);

	_currentRightAxisValue = FMath::Lerp(_currentRightAxisValue, _targetRotateRightAxisValue, InterpolationKey);
	FRotator currentRotation = GetActorRotation();
	float yawRotation = RotationSpeed * _currentRightAxisValue * DeltaTime;
	yawRotation = currentRotation.Yaw + yawRotation;

	FRotator newRotation = FRotator(0, yawRotation, 0);
	SetActorRotation(newRotation);

	// Turret rotation
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePos();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator currRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = currRotation.Pitch;
		targetRotation.Roll = currRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey));
	}

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