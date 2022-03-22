// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "BaseEnemyCharacter.h"
#include "Engine/World.h"

#include "EnemySpawner.generated.h"

UCLASS()
class ALPHABUILD_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
		int amount = 1;

	int X1, X2, Y1, Y2;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Base;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* SpawnRange;

};
