// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//INITIALIZING
	PlayerCapsule = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	PlayerSwordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerSwordComp"));
	DamageCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageCollider"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sword(TEXT("SkeletalMesh'/Game/Assets/3D_Assets/Sword/sword.sword''"));
	USkeletalMeshComponent* PlayerMeshComp = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	VaultTriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("VaultTriggerComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraArmX = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamArmX"));
	CameraArmZ = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamArmZ"));
	SurroundLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("SurroundLight"));
	DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComp"));

	//ANIMATIONS
	static ConstructorHelpers::FObjectFinder<UAnimSequence> attackSeq(TEXT("AnimSequence'/Game/Assets/3D_Assets/PlayerCharacter/PlayerAnimation/PlayerModel_Attacking_Sequence.PlayerModel_Attacking_Sequence'"));
	AttackSequence = attackSeq.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> idleSeq(TEXT("AnimSequence'/Game/Assets/3D_Assets/PlayerCharacter/PlayerAnimation/PlayerModel_Idle_Sequence.PlayerModel_Idle_Sequence'"));
	IdleSequence = idleSeq.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> specialSeq(TEXT("AnimSequence'/Game/Assets/3D_Assets/PlayerCharacter/PlayerAnimation/PlayerModel_Special_Sequence.PlayerModel_Special_Sequence'"));
	SpecialSequence = specialSeq.Object;

	//ATTACK SETUP
	PlayerSwordComp->SetStaticMesh(sword.Object);
	PlayerSwordComp->SetSimulatePhysics(false);
	PlayerSwordComp->SetGenerateOverlapEvents(false);
	PlayerSwordComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerSwordComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("SwordSocket"));

	DamageCollider->SetSimulatePhysics(false);
	DamageCollider->SetGenerateOverlapEvents(true);
	DamageCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageCollider->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("DamageCollider"));
	DamageCollider->ComponentTags.Add("pAttack");

	//ATTACHING
	DetectionComp->SetupAttachment(RootComponent);
	SurroundLight->SetupAttachment(RootComponent);
	VaultTriggerComp->SetupAttachment(RootComponent);
	CameraArmZ->SetupAttachment(RootComponent);
	CameraArmX->SetupAttachment(CameraArmZ);
	CameraComp->SetupAttachment(CameraArmX);

	//TRIGGERS SETUP
	RootComponent->ComponentTags.Add(TEXT("Player"));

	DetectionComp->SetGenerateOverlapEvents(true);
	DetectionComp->SetRelativeScale3D(FVector(30, 30, 30));
	DetectionComp->SetCollisionProfileName(TEXT("DetectTrigger"));
	DetectionComp->ComponentTags.Add(TEXT("pDetection"));
	DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnDetectBegin);
	DetectionComp->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnDetectEnd);

	VaultTriggerComp->SetRelativeLocation(FVector(25.f, 0, 0));
	VaultTriggerComp->SetRelativeScale3D(FVector(1, 0.5f, 0.5f));
	VaultTriggerComp->SetCollisionProfileName(TEXT("VaultTrigger"));
	VaultTriggerComp->SetGenerateOverlapEvents(true);
	VaultTriggerComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnVaultTriggerBegin);
	VaultTriggerComp->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnVaultTriggerEnd);

	//LIGHT SETUP
	SurroundLight->bUseInverseSquaredFalloff = false;
	SurroundLight->Intensity = 2.f;
	SurroundLight->AttenuationRadius = 500;
	SurroundLight->CastShadows = false;

	//MESH SETUP
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(TEXT("SkeletalMesh'/Game/Assets/3D_Assets/PlayerCharacter/PlayerModel.PlayerModel'"));
	PlayerMeshComp->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	PlayerMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -86.f));
	PlayerMeshComp->SetSkeletalMesh(MeshObject.Object);

	//CAMERA ARM SETUP
	//Z
	CameraArmZ->bUsePawnControlRotation = false;
	CameraArmZ->TargetArmLength = 1500.0f;
	CameraArmZ->AddWorldRotation(FRotator(-90, 0, 0));
	//X
	CameraArmX->bUsePawnControlRotation = true;
	CameraArmX->TargetArmLength = 1000.0f;
	CameraArmX->bEnableCameraLag = true;
	CameraArmX->CameraLagMaxDistance = 100;
	CameraArmX->CameraLagSpeed = 8;
	
	//CAMERA COMP SETUP
	CameraComp->bUsePawnControlRotation = false;
	CameraComp->SetRelativeRotation(FRotator(-55.f, 0, 0));

	//CHARACTER MOVEMENT SETUP
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	pEnemyInRange = false;
	isTargeting = false;
	GetMesh()->PlayAnimation(IdleSequence, true);

	pMaxHealth = 1000;
	pCurrentHealth = pMaxHealth;
	pUpdateHealth(-50);

	pMaxStamina = 10000;
	pCurrentStamina = pMaxStamina;
	pUpdateStamina(0);

	pMaxStress = 1000;
	pUpdateStress(10);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pTargetedEnemy != NULL)
	{
		FRotator EnemyLoc = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), pTargetedEnemy->GetActorLocation());
		this->SetActorRotation(FMath::RInterpTo(this->GetActorRotation(), EnemyLoc, DeltaTime, 90));
		DrawDebugLine(GetWorld(), this->GetActorLocation(), pTargetedEnemy->GetActorLocation(), FColor::Green, false, 0.1f, 0, 5);
	}

	if (!running && pCurrentStamina != pMaxStamina && pCurrentStress < 499)
	{
		pUpdateStamina(1);
	}
	else if (pCurrentStress >= 500)
	{
		pUpdateStamina(.6f);
	}

	if (pCurrentHealth < pMaxHealth && pCurrentStress < 499)
	{
		pUpdateHealth(.01f);
	}
	else if (pCurrentStress >= 500)
	{
		pUpdateHealth(-.01f);
	}

	if (pCurrentStress < 799)
	{
		pUpdateStress(.001f);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Controller_axis_left_x", this, &APlayerCharacter::MoveX);
	PlayerInputComponent->BindAxis("Controller_axis_left_y", this, &APlayerCharacter::MoveY);

	PlayerInputComponent->BindAction("b_button", IE_Pressed, this, &APlayerCharacter::StartRunning);
	PlayerInputComponent->BindAction("b_button", IE_Released, this, &APlayerCharacter::StopRunning);

	PlayerInputComponent->BindAction("a_button", IE_Pressed, this, &APlayerCharacter::Vaulting);
	PlayerInputComponent->BindAction("x_button", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("y_button", IE_Pressed, this, &APlayerCharacter::Targeting);
}


//STATS
float APlayerCharacter::pGetHealth()
{
	return pHealthPercentage;
}

float APlayerCharacter::pGetStress()
{
	return pStressPercentage;
}

float APlayerCharacter::pGetStamina()
{
	return pStaminaPercentage;
}

void APlayerCharacter::pUpdateStress(float StressChange)
{
	pCurrentStress += StressChange;
	pCurrentStress = FMath::Clamp(pCurrentStress, .0f, pMaxStress);
	pStressPercentage = pCurrentStress / pMaxStress;
}

void APlayerCharacter::pUpdateStamina(float StaminaChange)
{
	pCurrentStamina += StaminaChange;
	pCurrentStamina = FMath::Clamp(pCurrentStamina, .0f, pMaxStamina);
	pStaminaPercentage = pCurrentStamina / pMaxStamina;
}

void APlayerCharacter::pUpdateHealth(float HealthChange)
{
	pCurrentHealth += HealthChange;
	pCurrentHealth = FMath::Clamp(pCurrentHealth, .0f, pMaxHealth);
	pHealthPercentage = pCurrentHealth / pMaxHealth;
}

//MOVEMENT
void APlayerCharacter::MoveX(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	if (running && pCurrentStamina != 0)
	{
		Axis = Axis + Axis;
		pUpdateStamina(-1.5f);
	}
	AddMovementInput(ForwardDirection, Axis);
}

void APlayerCharacter::MoveY(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (running && pCurrentStamina != 0)
	{
		Axis = Axis + Axis;
		pUpdateStamina(-1.5f);
	}
	AddMovementInput(RightDirection, Axis);
}

//BUTTONS
void APlayerCharacter::StartRunning()
{
	running = true;
}

void APlayerCharacter::StopRunning()
{
	running = false;
}

void APlayerCharacter::Attack()
{
	GetMesh()->PlayAnimation(AttackSequence, false);
}

void APlayerCharacter::Targeting()
{
	if (!isTargeting)
	{
		if (pEnemyInRange)
		{
			float aDist = 1000;
			int eID = 0;
			for (int i = 0; i < pEnemiesInRange.Num(); i++)
			{
				FVector Start = GetActorLocation();
				FVector End = pEnemiesInRange[i]->GetActorLocation();
				if (FVector::Dist(Start, End) < aDist)
				{
					aDist = FVector::Dist(Start, End);
					eID = i;
				}
			}
			pTargetedEnemy = pEnemiesInRange[eID];
		}
		isTargeting = true;
	}
	else
	{
		isTargeting = false;
		pTargetedEnemy = NULL;
	}
}

void APlayerCharacter::Vaulting()
{
	if (CanVault) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Vaulting"));

		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		for (int counter = 100; counter != 0; counter--)
		{
			AddMovementInput(ForwardDirection, 1.0f);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("."));
		}
	}
}

//TRIGGERS
void APlayerCharacter::OnDetectBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Enemy")))
	{
		pEnemiesInRange.Add(OtherActor);
		pEnemyInRange = true;
	}
}

void APlayerCharacter::OnDetectEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(TEXT("Enemy")))
	{
		pEnemiesInRange.Remove(OtherActor);
		if (pEnemiesInRange.Num() == 0)
		{
			pEnemyInRange = false;
			isTargeting = false;
			pTargetedEnemy = NULL;
		}
	}
}

void APlayerCharacter::OnVaultTriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && OtherComp->ComponentHasTag(TEXT("Vaulting")))
	{
		CanVault = true;
	}
}

void APlayerCharacter::OnVaultTriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp && OtherComp->ComponentHasTag(TEXT("Vaulting")))
	{
		CanVault = false;
	}
}

void APlayerCharacter::GetDamage(float Damage, const UDamageType* DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection, AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo)
{
	pUpdateHealth(-Damage);
}