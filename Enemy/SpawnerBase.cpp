// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SpawnerBase.h"
#include "TimerManager.h"

// Sets default values
ASpawnerBase::ASpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnDelayHandle, this, &ASpawnerBase::SpawnEnemy, Delay, true);
}

void ASpawnerBase::SpawnEnemy()
{
	
	int RndEnemy = rand() % EnemiesToSpawn.Num();
	int RndSpawnPoint = rand() % SpawnPoints.Num();

	TSubclassOf<AActor> EnemyToSpawn = EnemiesToSpawn[RndEnemy];
	FVector SpawnLocation = SpawnPoints[RndSpawnPoint]->GetActorLocation();
	FRotator SpawnRotation = SpawnPoints[RndSpawnPoint]->GetActorRotation();

	GetWorld()->SpawnActor<AActor>(EnemyToSpawn, SpawnLocation, SpawnRotation);

}

// Called every frame
void ASpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

