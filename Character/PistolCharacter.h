// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PistolCharacter.generated.h"

UCLASS()
class TRAININGCAMP_API APistolCharacter : public ACharacter
{
	GENERATED_BODY()



///////////////////////////// DEFAULT COMPONENTS /////////////////////////////////////
public:
	APistolCharacter();

	//Componente Camara
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* PistolCameraComponent;

	//Mesh para pistola
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* PistolMesh;

	//Punto donde sale el disparo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USceneComponent* FireLocation;

	//Retícula de arma
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* AimingHelper;



//////////////////////////// FUNCIONES INTERNAS ///////////////////////////////////////
protected:
// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Casteo del rayo
	void RayTraceCast();

// Cambiar posición de la retícula
	void UpdateReticleByRayTrace();

// Modificar valor
	void ChangeHitValue();

// Movimiento de pistola vertical y horizontal
	void LookUp(float Val);
	void TurnRight(float Val);

// Disparo del arma
	void PistolShoot();

// Recargar nivel de agua
	void RechargeWaterLevel();

// Chequear nivel actual de agua
	void CheckWaterLevel();

// Nivel de agua actualizado todos los frames
	void UpdateGunWaterLevel(float Val);

// Emisión de partículas
	void SpawnEmitterAtLocation(const FVector& Location, const FRotator& Rotation);

// Reproducir sonido
	void PlaySoundAtLocation(USoundBase* SoundToPlay);



//////////////////////////// MANEJO DEL ARMA ///////////////////////////////////////
protected:
	
// Mapa de daño por huesos
	UPROPERTY(EditAnywhere, Category = "Damage Stats")
		TMap<FName, float> BoneDamage;

// Daño del arma
	UPROPERTY(EditAnywhere, Category = "Damage Stats")
		float WeaponDamage = 1.0f;

// Cantidad de agua que hasta el arma por disparo
	UPROPERTY(EditAnywhere, Category = "Damage Stats")
		float RegularShot = 100.0f;

// Nivel de agua actual
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
		float GunWaterLevel = 100.0f;

// Nivel máximo de agua
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
		float MaxWaterLevel = 100.0f;

// Velocidad de recarga del agua
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
		float WaterLevelUpdateVelocity = 1.0f;

// Contador de pociones de agua
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
		float WaterPotions = 0;

	//Puntos de jugador (similar a vida)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Condition")
		float PlayerPoints = 15.0f;



//////////////////////////// SONIDOS Y PARTÍCULAS ///////////////////////////////////////
protected:

// Disparo arma
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* FireSound;

// Recarga de agua
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* ReloadAmmoSound;

// Daño recibido
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* TakeDamageSound;

// No ammo
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* NoAmmoSound;

// Partículas de agua
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* ParticleToSpawn;



////////////////////////////  PLAYER  ///////////////////////////////////////
protected:

// Manejo del daño recibido por el player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage Stats")
	bool bPlayerTakeDmg;

// Timer para cambiar el valor del hit
	FTimerHandle HitPlayer;
	float HitPlayerDelay = 0.5f;
	
// Casteo del rayo para disparo
	FHitResult ShootingTrace() const;
	float TraceRange = 5000.0f;

// Determina si puede disparar
	bool bCanShoot = true;



//////////////////////////// FUNCIONES REFERENCIADAS DESDE OTRAS CLASES  ///////////////////////////////////////
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Recibir daño del enemigo
	void PlayerReceiveDamage(float Dmg);

	// Suma al contador de pociones
	void AddWaterPotion(float Val);


};
