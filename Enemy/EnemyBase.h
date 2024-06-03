// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "EnemyBase.generated.h"

UCLASS()
class TRAININGCAMP_API AEnemyBase : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float EnemyHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageTaken;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> WayPoints;

	UPROPERTY(BlueprintReadOnly)
	bool isMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Condition")
	float EnemyPoints = 15.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Win Condition")
	float WeaponDamage = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bGetHit = false;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* TakeDamageSound;

protected:
	//Creación del timer para el sistema de estadísticas
	FTimerHandle TimerHandle;
	float TimerDelay = 1.0f;
	void SetHitToFalse();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Dmg) override;

	void OrientRotationToMovement();

	UFUNCTION(BlueprintCallable)
	void ShootPlayer();

	float TraceRange = 5000.0f;

	void PlaySoundAtLocation(USoundBase* SoundToPlay);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
