// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Components 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "MyDirt.generated.h"

UCLASS()
class PROJECT_CLEAN_API AMyDirt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDirt();

	/* Components */

	// Static Mesh 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// Box Collider
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollider;

	/* Variables */
	// Points
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	float Points;

	/* Functions */
	UFUNCTION(BlueprintCallable, Category = "Dirt")
	void CollectDirt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
