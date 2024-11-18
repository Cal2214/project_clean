// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "MyDirtSpawner.generated.h"

class AMyDirt;

UCLASS()
class PROJECT_CLEAN_API AMyDirtSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDirtSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Functions */
	// Init Dirt
	void DelayDirt();
	void SpawnDirt();

	/* Variables */
	float SpawnTime;
	FTimerHandle MyTimerHandler;

	/* Refrences */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AMyDirt> DirtClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
