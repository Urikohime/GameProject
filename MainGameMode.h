// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerCharacter.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UENUM()
enum class EGamePlayState
{
	EPlaying, EGameOver, EUnknown
};

UCLASS()
class ALPHABUILD_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	APlayerCharacter* PlayerChar;

	

	UFUNCTION(Category = "Health")
		EGamePlayState GetCurrentState() const;

	void setCurrentState(EGamePlayState NewState);

private:
	EGamePlayState CurrentState;

	void HandleNewState(EGamePlayState NewState);
	
};
