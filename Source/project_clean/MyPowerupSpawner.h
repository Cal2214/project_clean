// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "MyPowerupSpawner.generated.h"

class AMyPowerup;

UCLASS()
class PROJECT_CLEAN_API AMyPowerupSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPowerupSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Functions */
	// Init Powerup
	void DelayPowerup();
	void SpawnPowerup();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DelayPowerup();
	bool Server_DelayPowerup_Validate();
	void Server_DelayPowerup_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SpawnPowerup(FVector RandomPoint);
	void Multi_SpawnPowerup_Implementation(FVector RandomPoint);

	/* Variables */
	float SpawnTime;
	FTimerHandle MyTimerHandler;

	/* Refrences */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AMyPowerup> PowerupClass;

	UFUNCTION(BlueprintCallable)
	void StartSpawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
