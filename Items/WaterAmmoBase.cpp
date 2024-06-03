// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WaterAmmoBase.h"
#include "Character/PistolCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonPC.h"

// Sets default values
AWaterAmmoBase::AWaterAmmoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaterAmmoBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWaterAmmoBase::GetPlayer(AActor* Player)
{
	APistolCharacter* PlayerRef = Cast<APistolCharacter>(Player);
	if (PlayerRef)
		PlayerRef->AddWaterPotion(1.0f);

	if (TakePotionSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TakePotionSound, GetActorLocation());

	Destroy();
}


// Called every frame
void AWaterAmmoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

