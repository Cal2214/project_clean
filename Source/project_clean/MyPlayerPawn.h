// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// Components 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "MyPlayerPawn.generated.h"

UCLASS()
class PROJECT_CLEAN_API AMyPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayerPawn();

	/* Components */

	// Static Mesh 
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	// Box Collider
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;

	// Camera
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	// SpringArm
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;


	/* Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
