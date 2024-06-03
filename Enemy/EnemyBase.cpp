// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"
#include "TrainingCamp/TrainingCampGameModeBase.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PistolCharacter.h"

#define COLLISION		ECC_GameTraceChannel3
///////////////////////////////////////////////////////////////
// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GetRootComponent());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CapsuleComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(SkeletalMesh, TEXT("RightHandWeapon"));

	// Vida inicial del enemigo
	EnemyHP = 1000.f;
}



// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetVelocity().Size() > 0)
	{
		isMoving = true;
		OrientRotationToMovement();
	}
	else
		isMoving = false;
}
///////////////////////////////////////////////////////////////


void AEnemyBase::OrientRotationToMovement()
{
	FRotator NewRotation = GetVelocity().Rotation();

	SetActorRotation(NewRotation);
}


void AEnemyBase::ShootPlayer()
{
	if (bGetHit == false)
	{
		FHitResult HitInfo;
		FVector LineStart = WeaponMesh->GetComponentLocation();
		FVector Forward = GetActorForwardVector();
		FVector LineEnd = LineStart + Forward * TraceRange;

		GetWorld()->LineTraceSingleByChannel(HitInfo, LineStart, LineEnd, COLLISION);
		DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Cyan, false, 0.3f, 0, 2);

		if (FireSound)
			PlaySoundAtLocation(FireSound);

		APistolCharacter* PlayerRef = Cast<APistolCharacter>(HitInfo.Actor);
		if (PlayerRef)
		{
			PlayerRef->PlayerReceiveDamage(WeaponDamage);
		}
	}

}


// Input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Daño que recibe el personaje
void AEnemyBase::TakeDamage(float Dmg)
{
	bGetHit = true;
	EnemyPoints -= Dmg;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::SetHitToFalse, TimerDelay, false);

	if (TakeDamageSound)
		PlaySoundAtLocation(TakeDamageSound);

	if (EnemyPoints <= 0)
	{
		ATrainingCampGameModeBase* GameModeRef = Cast<ATrainingCampGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameModeRef)
			GameModeRef->PlayerVictory();
	}
}


void AEnemyBase::SetHitToFalse()
{
	bGetHit = false;
}


void AEnemyBase::PlaySoundAtLocation(USoundBase* SoundToPlay)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());
}