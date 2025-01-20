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

	// Powerups
	void ActivateLeftPowerup(const FInputActionValue& Value);
	void ActivateRightPowerup(const FInputActionValue& Value);

	void MovePlayer();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MovePlayer(FVector NewLocation);
	bool Server_MovePlayer_Validate(FVector NewLocation);
	void Server_MovePlayer_Implementation(FVector NewLocation);

	UFUNCTION(BlueprintCallable, Category = "Points")
	void AddPoints(int32 Ammount);

	int32 GetPoints();

	// Powerups 
	UFUNCTION(BlueprintCallable, Category = "Powerup")
	void GetPowerUp(AMyPowerup* Powerup);


	void AssignPowerup(AMyPowerup* Powerup, bool bIsLeft);

	void UsePowerup();

	void ActivateSpeedPowerup();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ActivateSpeedPowerup(UMaterial* material);
	bool Server_ActivateSpeedPowerup_Validate(UMaterial* material);
	void Server_ActivateSpeedPowerup_Implementation(UMaterial* material);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ActivateSpeedPowerup(UMaterial* material);
	bool Multi_ActivateSpeedPowerup_Validate(UMaterial* material);
	void Multi_ActivateSpeedPowerup_Implementation(UMaterial* material);

	void ActivateSizePowerup();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ActivateSizePowerup(UMaterial* material, FVector scale);
	bool Server_ActivateSizePowerup_Validate(UMaterial* material, FVector scale);
	void Server_ActivateSizePowerup_Implementation(UMaterial* material, FVector scale);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ActivateSizePowerup(UMaterial* material, FVector scale);
	bool Multi_ActivateSizePowerup_Validate(UMaterial* material, FVector scale);
	void Multi_ActivateSizePowerup_Implementation(UMaterial* material, FVector scale);

	void DeactivateSpeedPowerup();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DeactivateSpeedPowerup(UMaterial* material);
	bool Server_DeactivateSpeedPowerup_Validate(UMaterial* material);
	void Server_DeactivateSpeedPowerup_Implementation(UMaterial* material);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_DeactivateSpeedPowerup(UMaterial* material);
	bool Multi_DeactivateSpeedPowerup_Validate(UMaterial* material);
	void Multi_DeactivateSpeedPowerup_Implementation(UMaterial* material);

	void DeactivateSizePowerup();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DeactivateSizePowerup(UMaterial* material, FVector scale);
	bool Server_DeactivateSizePowerup_Validate(UMaterial* material, FVector scale);
	void Server_DeactivateSizePowerup_Implementation(UMaterial* material, FVector scale);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_DeactivateSizePowerup(UMaterial* material, FVector scale);
	bool Multi_DeactivateSizePowerup_Validate(UMaterial* material, FVector scale);
	void Multi_DeactivateSizePowerup_Implementation(UMaterial* material, FVector scale);

	void RemoveLeftPowerup();
	void RemoveRightPowerup();

	/* Variables */
	// Movement
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(Replicated)
	FVector MovementDirection;

	// Score
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 PlayerScore;

	// Powerup 
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool hasPowerup1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool hasPowerup2;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool hasMaxPowerup;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool usePowerup1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool usePowerup2;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool hasSpeedLeft;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool hasSpeedRight;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool hasSizeLeft;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Powerups")
	bool hasSizeRight;

	float DelayTime;
	FTimerHandle MyTimerHandler;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterial* OriginalMaterial;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterial* SpeedMaterial;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EDirection CurrentDirection;
private:
};
