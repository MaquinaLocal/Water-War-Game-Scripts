// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AffectPlayer.h"
#include "FirstPersonPC.h"
#include "WaterAmmoBase.generated.h"

UCLASS()
class TRAININGCAMP_API AWaterAmmoBase : public AActor, public IAffectPlayer
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterAmmoBase();

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* TakePotionSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetPlayer(AActor* Player) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
