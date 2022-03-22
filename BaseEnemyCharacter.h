// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimSequence.h"
#include "Components/CapsuleComponent.h"

#include "BaseEnemyCharacter.generated.h"

UCLASS()
class ALPHABUILD_API ABaseEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* EnemyCapsule;

	//TRIGGERS
	UFUNCTION()
		void OnDetectBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDetectEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnDamagedStart(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDamagedEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void GetDamage(float Damage);

	//STATS
	UPROPERTY(EditAnywhere, Category = "Health")
		float eMaxHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
		float eHealthPercentage;

	UPROPERTY(EditAnywhere, Category = "Health")
		float eCurrentHealth;

	UFUNCTION(Category = "Health")
		void UpdateHealth(float HealthChange);

	UPROPERTY(VisibleAnywhere)
		class UAnimSequence* DeathSequence;

	UPROPERTY(VisibleAnywhere)
		class UAnimSequence* IdleSequence;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* DetectionComp;

	UPROPERTY(VisibleAnywhere)
		class AActor* PlayerActor;

	float MovementSpeed;
	bool GettingDamaged;
	bool PlayerInRange;
	bool IsDead;
};
