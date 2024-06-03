// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AffectPlayer.h"
#include "FriendlyTarget.generated.h"

UCLASS()
class TRAININGCAMP_API AFriendlyTarget : public APawn, public IAffectPlayer
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFriendlyTarget();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Character")
	float WalkSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Character")
	float HitRotation = 60.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetPlayer(AActor* Player) override;

	UPROPERTY(BlueprintReadOnly)
	bool bGetHit = false;

	FTimerHandle AnimTimer;
	UPROPERTY(EditAnywhere, Category = "Character")
	float AnimDelay = 4.0f;
	void ChangeHitValue();

	FTimerHandle MovementTimer;
	UPROPERTY(EditAnywhere, Category = "Character")
	float MovDelay = 15.0f;
	void ChangeDirection();

	float WalkRotation = 180.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
