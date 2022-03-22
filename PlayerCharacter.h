// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class ALPHABUILD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//TRIGGERS
	UFUNCTION()
		void OnDetectBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDetectEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnVaultTriggerBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnVaultTriggerEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void GetDamage(float Damage, const UDamageType* DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection, AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo);

	//PLAYER COMPONENTS
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* DetectionComp;

	UPROPERTY(VisibleAnywhere)
		class UPointLightComponent* SurroundLight;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* PlayerCapsule;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* PlayerSwordComp;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* DamageCollider;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* VaultTriggerComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraArmX;

	UPROPERTY(VisibleAnywhere, Category = Camery)
		class USpringArmComponent* CameraArmZ;

	//TARGETING
	UPROPERTY(VisibleAnywhere)
		AActor* pTargetedEnemy;

	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> pEnemiesInRange;

	//STATS
	UPROPERTY(VisibleAnywhere)
		bool pEnemyInRange;

	UFUNCTION(BlueprintPure, Category = "Health")
		float pGetHealth();

	UFUNCTION(Category = "Health")
		void pUpdateHealth(float HealthChange);

	UFUNCTION(BlueprintPure, Category = "Stress")
		float pGetStress();

	UFUNCTION(Category = "Stress")
		void pUpdateStress(float StressChange);

	UFUNCTION(BlueprintPure, Category = "Stamina")
		float pGetStamina();

	UFUNCTION(Category = "Stamina")
		void pUpdateStamina(float StaminaChange);

	UPROPERTY(EditAnywhere, Category = "Stress")
		float pMaxStress;

	UPROPERTY(EditAnywhere, Category = "Stress")
		float pStressPercentage;

	UPROPERTY(EditAnywhere, Category = "Stress")
		float pCurrentStress;

	UPROPERTY(EditAnywhere, Category = "Health")
		float pMaxHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
		float pHealthPercentage;

	UPROPERTY(EditAnywhere, Category = "Health")
		float pCurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		float pMaxStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		float pStaminaPercentage;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		float pCurrentStamina;

	//ANIMATIONS
	UPROPERTY(VisibleAnywhere)
		class UAnimNotify* pAttackNotify;

	UPROPERTY(VisibleAnywhere)
		class UAnimSequence* IdleSequence;

	UPROPERTY(VisibleAnywhere)
		class UAnimSequence* AttackSequence;

	UPROPERTY(VisibleAnywhere)
		class UAnimSequence* SpecialSequence;

	//BOOLS
	bool isTargeting;
	bool running;
	bool CanVault;

	//MOVEMENT FUNCTIONS
	void MoveX(float Axis);
	void MoveY(float Axis);

	//BUTTON FUNCTIONS
	//B
	void StartRunning();
	void StopRunning();
	//A
	void Vaulting();
	//Y
	void Targeting();
	//X
	void Attack();
};
