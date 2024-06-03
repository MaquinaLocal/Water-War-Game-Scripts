// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerBase.generated.h"

UCLASS()
class TRAININGCAMP_API ASpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerBase();
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> EnemiesToSpawn;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere)
	float Delay = 3.0f;
	FTimerHandle SpawnDelayHandle;

	int8 iteration = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
