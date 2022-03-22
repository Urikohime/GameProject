// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnRange = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRange"));
	Base = CreateDefaultSubobject<UBoxComponent>(TEXT("Base"));

	Base->SetupAttachment(RootComponent);
	Base->SetRelativeLocation(FVector(0, 0, 0));

	SpawnRange->SetupAttachment(Base);
	SpawnRange->SetRelativeLocation(FVector(0, 0, 33));
	SpawnRange->SetRelativeScale3D(FVector(20, 20, 1));
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	X1 = RootComponent->GetRelativeLocation().X;
	Y1 = RootComponent->GetRelativeLocation().Y;
	X2 = (SpawnRange->GetRelativeScale3D().X / 2) * 65;
	Y2 = (SpawnRange->GetRelativeScale3D().Y / 2) * 65;

	int Xs, Ys, Xe, Ye;

	Xs = X1 - X2;
	Ys = Y1 - Y2;

	Xe = X1 + X2;
	Ye = Y1 + Y2;

	for (int I = amount; amount > 0; amount--)
	{
		FTransform SpawnLocation;
		SpawnLocation.SetLocation(FVector(FMath::RandRange(Xs, Xe), FMath::RandRange(Ys, Ye), 0));
		UObject* BPObject = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Blueprints/BaseEnemyCharacter_BP.BaseEnemyCharacter_BP'")));
		UBlueprint* BP = Cast<UBlueprint>(BPObject);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		AActor* Spawned = GetWorld()->SpawnActor<ABaseEnemyCharacter>(BP->GeneratedClass, SpawnLocation, Params);
	}
}