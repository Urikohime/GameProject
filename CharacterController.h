// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CharacterController.generated.h"

UCLASS()
class ALPHABUILD_API ACharacterController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharacterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* CharacterCapsule;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
		class UCharacterMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere)
		class UInputComponent* PlayerInputComp;

	void MoveX(float Axis);
	void MoveY(float Axis);
};
