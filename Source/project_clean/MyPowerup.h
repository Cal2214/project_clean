// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Components 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "MyPowerup.generated.h"

UENUM(BlueprintType)
enum class EPowerupType : uint8
{
	Speed UMETA(DisplayName = "Speed"),
	Size  UMETA(DisplayName = "Size"),
};

UCLASS()
class PROJECT_CLEAN_API AMyPowerup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPowerup();

	/* Components */

	// Static Mesh 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// Box Collider
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollider;

	/* Functions */
	// Get Powerup Type
	UFUNCTION(BlueprintCallable, Category = "Powerup")
	EPowerupType GetPowerup();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentPowerup")
	EPowerupType CurrentPowerup;
};
