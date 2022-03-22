// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

// Sets default values
ACharacterController::ACharacterController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//INITIALIZING
	PlayerInputComp = CreateAbstractDefaultSubobject<UInputComponent>(TEXT("PlayerInput"));
	CharacterCapsule = CreateAbstractDefaultSubobject<UCapsuleComponent>(TEXT("Character"));
	CameraComp = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraArm = CreateAbstractDefaultSubobject<USpringArmComponent>(TEXT("CamArm"));
	MovementComp = CreateAbstractDefaultSubobject<UCharacterMovementComponent>(TEXT("Movement"));

	SetupPlayerInputComponent(PlayerInputComp);

	//ATTACHING
	CameraComp->SetupAttachment(CameraArm);
	CameraArm->SetupAttachment(CharacterCapsule);

	//CAMERA ARM SETUP
	CameraArm->TargetArmLength = 1000.0f;
	CameraArm->SetRelativeRotation(FRotator(-45.0f, 0, 0));
}

// Called when the game starts or when spawned
void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

// Called to bind functionality to input
void ACharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Controller_axis_left_x", this, &ACharacterController::MoveX);
	PlayerInputComponent->BindAxis("Controller_axis_left_y", this, &ACharacterController::MoveY);
}

void ACharacterController::MoveX(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Axis);
}

void ACharacterController::MoveY(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, Axis);
}