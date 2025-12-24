// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerPawn.h"

#include "MyPowerup.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AMyPlayerPawn::AMyPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetMaterial(0, OriginalMaterial);
	RootComponent = StaticMeshComponent;

	HoodComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoodComponent"));
	HoodComponent->SetupAttachment(RootComponent);
	HoodComponent->SetIsReplicated(true);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderComponent"));
	BoxCollider->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	MoveSpeed = 3.0f;
	CurrentDirection = EDirection::Still;
	PlayerScore = 0;

	bReplicates = true; // Ensures the actor replicates
	hasPowerup1 = false;
	hasPowerup2 = false;
	hasMaxPowerup = false;
	usePowerup1 = false;
	usePowerup2 = false;
	hasSpeedLeft = false;
	hasSpeedRight = false;
	hasSizeLeft = false;
	hasSizeRight = false;
}

// Called when the game starts or when spawned
void AMyPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetupInputMappingContext();
}

// Called every frame
void AMyPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlayer();

	// Test
	// Set max power-up flag if both slots are filled
	if (hasPowerup1 && hasPowerup2)
	{
		hasMaxPowerup = true;
	}
	else {
		hasMaxPowerup = false;
	}
}

// Called to bind functionality to input
void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement 
		EnhancedInputComponent->BindAction(RightMove, ETriggerEvent::Started, this, &AMyPlayerPawn::MoveRightInput);
		EnhancedInputComponent->BindAction(LeftMove, ETriggerEvent::Started, this, &AMyPlayerPawn::MoveLeftInput);
		EnhancedInputComponent->BindAction(UpMove, ETriggerEvent::Started, this, &AMyPlayerPawn::MoveUpInput);
		EnhancedInputComponent->BindAction(DownMove, ETriggerEvent::Started, this, &AMyPlayerPawn::MoveDownInput);
		EnhancedInputComponent->BindAction(LeftPowerup, ETriggerEvent::Started, this, &AMyPlayerPawn::ActivateLeftPowerup);
		EnhancedInputComponent->BindAction(RightPowerup, ETriggerEvent::Started, this, &AMyPlayerPawn::ActivateRightPowerup);
	}
}

void AMyPlayerPawn::SetupInputMappingContext()
{
	// Set Mapping Context 
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

// Move Left
void AMyPlayerPawn::MoveLeftInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Left;
		RotateHood();
	}
}

// Move Right
void AMyPlayerPawn::MoveRightInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Right;
		RotateHood();
	}
}

// Move Up
void AMyPlayerPawn::MoveUpInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Up;
		RotateHood();
	}
}

// Move Down
void AMyPlayerPawn::MoveDownInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Down;
		RotateHood();
	}
}

void AMyPlayerPawn::MovePlayer()
{
	//RotateHood();

	if (HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Server is handling movement."));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Client is calling server move."));
	}

	FVector NewLocation = GetActorLocation();
	FVector Reverse = FVector::ZeroVector;

	switch (CurrentDirection)
	{
	case EDirection::Right:
		NewLocation.Y += MoveSpeed;
		break;
	case EDirection::Left:
		Reverse = GetActorRightVector() * -1.0;
		NewLocation.Y += Reverse.Y * MoveSpeed;
		break;
	case EDirection::Up:
		NewLocation.X += MoveSpeed;
		break;
	case EDirection::Down:
		Reverse = GetActorForwardVector() * -1.0;
		NewLocation.X += Reverse.X * MoveSpeed;
		break;
	default:
		break;
	}

	SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

	if (!HasAuthority())
	{
		Server_MovePlayer(NewLocation);
	}
}

bool AMyPlayerPawn::Server_MovePlayer_Validate(FVector NewLocation)
{
	return true;
}

void AMyPlayerPawn::Server_MovePlayer_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);
}

void AMyPlayerPawn::RotateHood()
{
	switch (CurrentDirection)
	{
	case EDirection::Right:
		HoodRotation = FRotator(0, 0, 0);
		break;
	case EDirection::Left:
		HoodRotation = FRotator(0, 180, 0);
		break;
	case EDirection::Up:
		HoodRotation = FRotator(0, -90, 0);
		break;
	case EDirection::Down:
		HoodRotation = FRotator(0, 90, 0);
		break;
	default:
		break;
	}

	Server_RotateHood(HoodRotation);
}

bool AMyPlayerPawn::Server_RotateHood_Validate(FRotator NewRotation)
{
	return true; 
}

void AMyPlayerPawn::Server_RotateHood_Implementation(FRotator NewRotation)
{
	Multi_RotateHood(NewRotation);
}

bool AMyPlayerPawn::Multi_RotateHood_Validate(FRotator NewRotation)
{
	return true;
}

void AMyPlayerPawn::Multi_RotateHood_Implementation(FRotator NewRotation)
{
	HoodRotation = NewRotation;
	OnRep_HoodRotation();
}

void AMyPlayerPawn::OnRep_HoodRotation()
{
	if (HoodComponent)
	{
		HoodComponent->SetRelativeRotation(HoodRotation);
	}
}

void AMyPlayerPawn::AddPoints(int32 Ammount)
{
	PlayerScore += Ammount; 
}

int32 AMyPlayerPawn::GetPoints()
{
	return PlayerScore;
}

void AMyPlayerPawn::GetPowerUp(AMyPowerup* Powerup)
{
	if (Powerup == nullptr) return;

	if (hasMaxPowerup) {
		return;
	}

		// Assign the power-up to the first empty slot
		if (!hasPowerup1) // First try Slot 1
		{
			AssignPowerup(Powerup, true);
			hasPowerup1 = true;
		}
		else if (!hasPowerup2) // Then Slot 2
		{
			AssignPowerup(Powerup, false);
			hasPowerup2 = true;
		}
}

void AMyPlayerPawn::AssignPowerup(AMyPowerup* Powerup, bool bIsLeft)
{
	EPowerupType PowerupType = Powerup->GetPowerup(); 

	if (bIsLeft)
	{

		// Reset left slot state first
		hasSpeedLeft = false;
		hasSizeLeft = false;

		if (PowerupType == EPowerupType::Speed)
		{
			hasSpeedLeft = true;
		}
		else if (PowerupType == EPowerupType::Size)
		{
			hasSizeLeft = true;
		}
	}
	else
	{

		// Reset right slot state first
		hasSpeedRight = false;
		hasSizeRight = false;

		if (PowerupType == EPowerupType::Speed)
		{
			hasSpeedRight = true;
		}
		else if (PowerupType == EPowerupType::Size)
		{
			hasSizeRight = true;
		}
	}
}

void AMyPlayerPawn::ActivateLeftPowerup(const FInputActionValue& Value) 
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		if (hasPowerup1) {
			usePowerup1 = true;
				UsePowerup();
		}
		else {
			usePowerup1 = false;
		}
	}
}

void AMyPlayerPawn::ActivateRightPowerup(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		if (hasPowerup2) {
			usePowerup2 = true;
				UsePowerup(); 
		}
		else {
			usePowerup2 = false;
		}
	}
}

void AMyPlayerPawn::UsePowerup()
{
	// Handle left power-up activation
	if (usePowerup1) {
		if (hasSpeedLeft) {
			ActivateSpeedPowerup();
			RemoveLeftPowerup(); // Clear left power-up after use
		}
		else if (hasSizeLeft) {
			ActivateSizePowerup();
			RemoveLeftPowerup(); // Clear left power-up after use
		}
	}

	// Handle right power-up activation
	if (usePowerup2) {
		if (hasSpeedRight) {
			ActivateSpeedPowerup();
			RemoveRightPowerup(); // Clear right power-up after use
		}
		else if (hasSizeRight) {
			ActivateSizePowerup();
			RemoveRightPowerup(); // Clear right power-up after use
		}
	}
}

void AMyPlayerPawn::ActivateSpeedPowerup()
{
	// Temporarily increase player movement speed
	MoveSpeed = 10.0f;
	DelayTime = 5.0f;

	//Multi_ActivateSpeedPowerup(SpeedMaterial);
	Server_ActivateSpeedPowerup(SpeedMaterial);

	// Set a timer to deactivate the speed boost after DelayTime seconds
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandler, this, &AMyPlayerPawn::DeactivateSpeedPowerup, DelayTime, false);
}

bool AMyPlayerPawn::Server_ActivateSpeedPowerup_Validate(UMaterial* material)
{
	return true;
}

void AMyPlayerPawn::Server_ActivateSpeedPowerup_Implementation(UMaterial* material)
{
	Multi_ActivateSpeedPowerup(material);
}

bool AMyPlayerPawn::Multi_ActivateSpeedPowerup_Validate(UMaterial* material)
{
	return true;
}

void AMyPlayerPawn::Multi_ActivateSpeedPowerup_Implementation(UMaterial* material)
{
	StaticMeshComponent->SetMaterial(0, material);
}

void AMyPlayerPawn::ActivateSizePowerup()
{
	// Temporarily increase player size
	FVector Scale = FVector(0.7, 0.7, 0.3);
	DelayTime = 5.0f;

	//Multi_ActivateSizePowerup(OriginalMaterial, Scale);
	Server_ActivateSizePowerup(OriginalMaterial, Scale);

	// Set a timer to deactivate the size boost after DelayTime seconds
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandler, this, &AMyPlayerPawn::DeactivateSizePowerup, DelayTime, false);
}

bool AMyPlayerPawn::Server_ActivateSizePowerup_Validate(UMaterial* material, FVector scale)
{
	return true;
}

void AMyPlayerPawn::Server_ActivateSizePowerup_Implementation(UMaterial* material, FVector scale)
{
	Multi_ActivateSizePowerup(material, scale);
}

bool AMyPlayerPawn::Multi_ActivateSizePowerup_Validate(UMaterial* material, FVector scale)
{
	return true;
}

void AMyPlayerPawn::Multi_ActivateSizePowerup_Implementation(UMaterial* material, FVector scale)
{
	StaticMeshComponent->SetWorldScale3D(scale);
}

void AMyPlayerPawn::DeactivateSpeedPowerup()
{
	// Reset player movement speed to default value
	MoveSpeed = 3.0f;
	Server_DeactivateSpeedPowerup(OriginalMaterial);
	//Multi_DeactivateSpeedPowerup(OriginalMaterial);
}

bool AMyPlayerPawn::Server_DeactivateSpeedPowerup_Validate(UMaterial* material)
{
	return true;
}

void AMyPlayerPawn::Server_DeactivateSpeedPowerup_Implementation(UMaterial* material)
{
	Multi_DeactivateSpeedPowerup(material);
}

bool AMyPlayerPawn::Multi_DeactivateSpeedPowerup_Validate(UMaterial* material)
{
	return true;
}

void AMyPlayerPawn::Multi_DeactivateSpeedPowerup_Implementation(UMaterial* material)
{
	StaticMeshComponent->SetMaterial(0, material);
}

void AMyPlayerPawn::DeactivateSizePowerup()
{
	// Reset player size to default scale
	FVector Scale = FVector(0.5, 0.5, 0.3);
	
	Server_DeactivateSizePowerup(OriginalMaterial, Scale);
	//Multi_DeactivateSizePowerup(OriginalMaterial, Scale);
}

bool AMyPlayerPawn::Server_DeactivateSizePowerup_Validate(UMaterial* material, FVector scale)
{
	return true;
}

void AMyPlayerPawn::Server_DeactivateSizePowerup_Implementation(UMaterial* material, FVector scale)
{
	Multi_DeactivateSizePowerup(material, scale);
}

bool AMyPlayerPawn::Multi_DeactivateSizePowerup_Validate(UMaterial* material, FVector scale)
{
	return true;
}

void AMyPlayerPawn::Multi_DeactivateSizePowerup_Implementation(UMaterial* material, FVector scale)
{
	StaticMeshComponent->SetWorldScale3D(scale);
}

void AMyPlayerPawn::RemoveLeftPowerup()
{
	// Clear all state related to the left power-up slot
	hasPowerup1 = false;
	usePowerup1 = false;
	hasSpeedLeft = false;
	hasSizeLeft = false;
	// If both slots are empty, reset the max power-up flag
	if (!hasPowerup1 && !hasPowerup2)
	{
		hasMaxPowerup = false;
	}
}

void AMyPlayerPawn::RemoveRightPowerup()
{
	// Clear all state related to the right power-up slot
	hasPowerup2 = false;
	usePowerup2 = false;
	hasSpeedRight = false;
	hasSizeRight = false;
	// If both slots are empty, reset the max power-up flag
	if (!hasPowerup1 && !hasPowerup2)
	{
		hasMaxPowerup = false;
	}
}

void AMyPlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the movement direction
	DOREPLIFETIME(AMyPlayerPawn, CurrentDirection);
	DOREPLIFETIME(AMyPlayerPawn, MoveSpeed); 
	DOREPLIFETIME(AMyPlayerPawn, HoodRotation);

	// Replicate for the powerups
	DOREPLIFETIME(AMyPlayerPawn, hasPowerup1);
	DOREPLIFETIME(AMyPlayerPawn, hasPowerup2);
	DOREPLIFETIME(AMyPlayerPawn, hasMaxPowerup);
	DOREPLIFETIME(AMyPlayerPawn, usePowerup1);
	DOREPLIFETIME(AMyPlayerPawn, usePowerup2);
	DOREPLIFETIME(AMyPlayerPawn, hasSpeedLeft);
	DOREPLIFETIME(AMyPlayerPawn, hasSpeedRight);
	DOREPLIFETIME(AMyPlayerPawn, hasSizeLeft);
	DOREPLIFETIME(AMyPlayerPawn, hasSizeRight);

	// Scores
	DOREPLIFETIME(AMyPlayerPawn, PlayerScore);

	// Materials 
	DOREPLIFETIME(AMyPlayerPawn, OriginalMaterial);
	DOREPLIFETIME(AMyPlayerPawn, SpeedMaterial);
}