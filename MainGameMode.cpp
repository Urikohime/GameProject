// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AMainGameMode::AMainGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/PlayerCharacter_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<AHUD> Hud(TEXT("/Game/Blueprints/PlayerHUD_BP"));

	HUDClass = (UClass*)Hud.Class;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	setCurrentState(EGamePlayState::EPlaying);

	PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetWorld()->GetMapName();

	if (PlayerChar)
	{
		if (FMath::IsNearlyZero(PlayerChar->pGetHealth(), 0.001f))
		{
			setCurrentState(EGamePlayState::EGameOver);
		}
	}
}

EGamePlayState AMainGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AMainGameMode::setCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void AMainGameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
	{
		// do nothing
	}
	break;
	// Unknown/default state
	case EGamePlayState::EGameOver:
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
	break;
	// Unknown/default state
	default:
	case EGamePlayState::EUnknown:
	{
		// do nothing
	}
	break;
	}
}