// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// Components 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputAction.h"
#include "InputActionValue.h"

#include "MyPlayerPawn.generated.h"

class UInputMappingContext;
class AMyPowerup;

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Right UMETA(DisplayName = "Right"),
	Left  UMETA(DisplayName = "Left"),
	Up    UMETA(DisplayName = "Up"),
	Down  UMETA(DisplayName = "Down"),
	Still UMETA(DisplayName = "Still")
};

UCLASS()
class PROJECT_CLEAN_API AMyPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayerPawn();

	/* Components */

	// Static Mesh 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// Box Collider
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollider;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	// SpringArm
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Input Actions */

	// Mapping Context 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LeftMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RightMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UpMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DownMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LeftPowerup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RightPowerup;

	/* Functions */

	// SetUp InputMappingContext
	void SetupInputMappingContext();

	// Movement
	void MoveLeftInput(const FInputActionValue& Value);
	void MoveRightInput(const FInputActionValue& Value);
	void MoveUpInput(const FInputActionValue& Value);
	void MoveDownInput(const FInputActionValue& Value);
	void ActivateLeftPowerup(const FInputActionValue& Value);
	void ActivateRightPowerup(const FInputActionValue& Value);

	void MovePlayer();

	UFUNCTION(BlueprintCallable, Category = "Points")
	void AddPoints(int32 Ammount);

	int32 GetPoints();

	// Powerups 
	UFUNCTION(BlueprintCallable, Category = "Powerup")
	void GetPowerUp(AMyPowerup* Powerup);

	void AssignPowerup(AMyPowerup* Powerup, bool bIsLeft);
	void UsePowerup();

	void ActivateSpeedPowerup();
	void ActivateSizePowerup();

	void DeactivateSpeedPowerup();
	void DeactivateSizePowerup();

	void RemoveLeftPowerup();
	void RemoveRightPowerup();

	/* Variables */
	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	float MoveSpeed;

	FVector MovementDirection;

	// Score
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	int32 PlayerScore;

	// Powerup 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool hasPowerup1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool hasPowerup2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool hasMaxPowerup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool usePowerup1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool usePowerup2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool hasSpeedLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool hasSpeedRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool hasSizeLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool hasSizeRight;

	float DelayTime;
	FTimerHandle MyTimerHandler;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	EDirection CurrentDirection;
};
