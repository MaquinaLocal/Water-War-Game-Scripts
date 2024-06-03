// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PistolCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Character/ProjectileBase.h"
#include "TrainingCamp/TrainingCampGameModeBase.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Damageable.h"
#include "AffectPlayer.h"

#define COLLISION_WEAPON		ECC_GameTraceChannel1
////////////////////////////////////////////////////////////////////////////////////////
// Sets default values
APistolCharacter::APistolCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PistolCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PistolCamera"));
	PistolCameraComponent->SetRelativeLocation(FVector(0, 0, 64.f));
	PistolCameraComponent->SetupAttachment(GetCapsuleComponent());

	PistolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolMesh"));
	PistolMesh->SetupAttachment(PistolCameraComponent);

	FireLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FireLocation"));
	FireLocation->SetupAttachment(PistolCameraComponent);

	AimingHelper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aim"));
	AimingHelper->SetupAttachment(GetRootComponent());


}


// Called when the game starts or when spawned
void APistolCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APistolCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateReticleByRayTrace();

	UpdateGunWaterLevel(WaterLevelUpdateVelocity);

}
////////////////////////////////////////////////////////////////////////////////////////

//Actualiza posición de la retícula
void APistolCharacter::UpdateReticleByRayTrace()
{
	FVector LineStart = PistolMesh->GetComponentLocation();
	FVector Rotation = PistolMesh->GetForwardVector();
	FVector LineEnd = LineStart + -Rotation * TraceRange;

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_Visibility))
	{
		AimingHelper->SetWorldLocation(HitResult.ImpactPoint);

		//FRotator SurfaceRotation = HitResult.ImpactNormal.Rotation();
		//AimingHelper->SetWorldRotation(SurfaceRotation);
	}
}


//Inputs
void APistolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("LookUp", this, &APistolCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APistolCharacter::TurnRight);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APistolCharacter::PistolShoot);
	PlayerInputComponent->BindAction("UsePotion", IE_Pressed, this, &APistolCharacter::RechargeWaterLevel);
}


//Movimiento del arma vertical
void APistolCharacter::LookUp(float Val)
{
	if (Val != 0.0f)
	{
		FRotator NewRotation(Val, 0.0f, 0.0f);
		PistolMesh->AddRelativeRotation(NewRotation);
		FireLocation->AddRelativeRotation(NewRotation*-1);
	}
}


//Movimiento del arma horizontal
void APistolCharacter::TurnRight(float Val)
{
	if (Val != 0.0f)
	{
		FRotator NewRotation(0.0f, Val, 0.0f);
		PistolMesh->AddRelativeRotation(NewRotation);
		FireLocation->AddRelativeRotation(NewRotation);
	}
}


//Disparo de arma
void APistolCharacter::PistolShoot()
{
		
	if (GunWaterLevel >= MaxWaterLevel) {

		RayTraceCast();
		CheckWaterLevel();

		if (FireSound)
			PlaySoundAtLocation(FireSound);

	}
	else
	{
		if (NoAmmoSound)
			PlaySoundAtLocation(NoAmmoSound);
	}

}


//Controla nivel de agua
void APistolCharacter::CheckWaterLevel()
{
	GunWaterLevel -= RegularShot;
}

// Se actualiza cada frame
void APistolCharacter::UpdateGunWaterLevel(float Val)
{
	if (GunWaterLevel < MaxWaterLevel)
		GunWaterLevel += Val;
}

// Referencia desde WaterAmmoBase
void APistolCharacter::AddWaterPotion(float Val)
{
	WaterPotions += Val;
}


//Recarga nivel de agua
void APistolCharacter::RechargeWaterLevel()
{
	if (WaterPotions > 0)
	{
		GunWaterLevel = MaxWaterLevel;

		if (ReloadAmmoSound)
			PlaySoundAtLocation(ReloadAmmoSound);

		WaterPotions--;
	}	
}



//Castea a las interfaces de daño
void APistolCharacter::RayTraceCast()
{
	FHitResult Hit = ShootingTrace();

	SpawnEmitterAtLocation(Hit.Location, FRotator::ZeroRotator);

	IDamageable* DamageInterface = Cast<IDamageable>(Hit.Actor);
	if (DamageInterface && BoneDamage.Contains(Hit.BoneName))
	{
		float DamageValue = BoneDamage[Hit.BoneName];
		DamageInterface->TakeDamage(WeaponDamage * DamageValue);
	}
	else if (DamageInterface)
	{
		DamageInterface->TakeDamage(WeaponDamage);
	}

	IAffectPlayer* WaterInterface = Cast<IAffectPlayer>(Hit.Actor);
	if (WaterInterface)
	{
		WaterInterface->GetPlayer(this);
	}
}


//Castea el rayo a partir del disparo. Llamada desde RayTraceCast
FHitResult APistolCharacter::ShootingTrace() const
{
	FHitResult HitInfo;
	FVector LineStart = FireLocation->GetComponentLocation();
	FVector Forward = FireLocation->GetForwardVector();
	FVector LineEnd = LineStart + Forward * TraceRange;

	GetWorld()->LineTraceSingleByChannel(HitInfo, LineStart, LineEnd, COLLISION_WEAPON);

	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Cyan, false, 0.2f, 0, 1);

	return HitInfo;
}


//Jugador recibe daño
void APistolCharacter::PlayerReceiveDamage(float Dmg)
{
	PlayerPoints -= Dmg;

	if (TakeDamageSound)
		PlaySoundAtLocation(TakeDamageSound);

	bPlayerTakeDmg = true;
	GetWorld()->GetTimerManager().SetTimer(HitPlayer, this, &APistolCharacter::ChangeHitValue, HitPlayerDelay, false);

	//Chequea condición de derrota
	if (PlayerPoints <= 0)
	{
		ATrainingCampGameModeBase* GameModeRef = Cast<ATrainingCampGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameModeRef)
			GameModeRef->PlayerDefeat();
	}
}


//Emisión de partículas
void APistolCharacter::SpawnEmitterAtLocation(const FVector& Location, const FRotator& Rotation)
{
	if (ParticleToSpawn)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleToSpawn, Location, Rotation);
}


//Reproducción de sonidos
void APistolCharacter::PlaySoundAtLocation(USoundBase* SoundToPlay)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());
}


void APistolCharacter::ChangeHitValue()
{
	bPlayerTakeDmg = false;
}