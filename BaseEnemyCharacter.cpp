// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//INITILAIZING
	EnemyCapsule = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	USkeletalMeshComponent* EnemyMeshComp = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComp"));

	//ANIMATIONS
	static ConstructorHelpers::FObjectFinder<UAnimSequence> idleSeq(TEXT("AnimSequence'/Game/Assets/3D_Assets/EnemyCharacters/EnemyAnimation/BaseEnemyModel_Idle_Sequence.BaseEnemyModel_Idle_Sequence'"));
	IdleSequence = idleSeq.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> deathSeq(TEXT("AnimSequence'/Game/Assets/3D_Assets/EnemyCharacters/EnemyAnimation/BaseEnemyModel_Death_Sequence.BaseEnemyModel_Death_Sequence'"));
	DeathSequence = deathSeq.Object;

	//ATTACHING
	DetectionComp->SetupAttachment(RootComponent);

	//TRIGGERSETUP
	RootComponent->ComponentTags.Add(TEXT("Enemy"));

	EnemyCapsule->SetGenerateOverlapEvents(true);
	EnemyCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyCharacter::OnDamagedStart);
	EnemyCapsule->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemyCharacter::OnDamagedEnd);

	//DETECTOR SETUP
	DetectionComp->SetGenerateOverlapEvents(true);
	DetectionComp->SetRelativeLocation(FVector(0, 0, 0));
	DetectionComp->SetCollisionProfileName(TEXT("DetectTrigger"));
	DetectionComp->ComponentTags.Add(TEXT("eDetection"));
	DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyCharacter::OnDetectBegin);
	DetectionComp->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemyCharacter::OnDetectEnd);

	//MESH SETUP
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(TEXT("SkeletalMesh'/Game/Assets/3D_Assets/EnemyCharacters/BaseEnemyModel.BaseEnemyModel'"));
	EnemyMeshComp->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	EnemyMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -86.f));
	EnemyMeshComp->SetSkeletalMesh(MeshObject.Object);
	EnemyMeshComp->BodyInstance.bLockXRotation = true;
	EnemyMeshComp->BodyInstance.bLockYRotation = true;

	//MOVEMENT SETUP
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
	GettingDamaged = false;
	IsDead = false;

	this->SetActorRotation(FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f));

	MovementSpeed = FMath::RandRange(.3f, .45f);
	GetMesh()->PlayAnimation(IdleSequence, true);

	float randomDetect = FMath::RandRange(20, 45);
	DetectionComp->SetRelativeScale3D(FVector(randomDetect, randomDetect, randomDetect));

	eMaxHealth = 1000;
	eCurrentHealth = eMaxHealth;
}

// Called every frame
void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsDead)
	{
		if (PlayerInRange)
		{
			if (PlayerActor != NULL)
			{
				FRotator PlayerLoc = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), PlayerActor->GetActorLocation());

				this->SetActorRotation(FMath::RInterpTo(this->GetActorRotation(), PlayerLoc, DeltaTime, (MovementSpeed * 90)));
				this->AddMovementInput(this->GetActorForwardVector(), MovementSpeed, false);
			
			}
		}
	}
}

void ABaseEnemyCharacter::OnDetectBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Player")))
	{
		PlayerActor = OtherActor;
		PlayerInRange = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Player in Range"));
	}
}

void ABaseEnemyCharacter::OnDetectEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(TEXT("Player")))
	{
		PlayerActor = NULL;
		PlayerInRange = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Out of Range"));
	}
}

void ABaseEnemyCharacter::UpdateHealth(float HealthChange)
{
	eCurrentHealth += HealthChange;
	eCurrentHealth = FMath::Clamp(eCurrentHealth, .0f, eMaxHealth);
	eHealthPercentage = eCurrentHealth / eMaxHealth;
}

void ABaseEnemyCharacter::OnDamagedEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(TEXT("pAttack")))
	{
		GettingDamaged = false;
	}
}

void ABaseEnemyCharacter::OnDamagedStart(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("pAttack") && !GettingDamaged)
	{
		GetDamage(100);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("EMOTIONAL DAMAGE"));
	}

	if (eCurrentHealth <= 0)
	{
		GetMesh()->PlayAnimation(DeathSequence, false);
		RootComponent->Deactivate();
		IsDead = true;
		EnemyCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetectionComp->Deactivate();
	}
	GettingDamaged = true;
}

void ABaseEnemyCharacter::GetDamage(float Damage)
{
	UpdateHealth(-Damage);
}