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
	RootComponent = StaticMeshComponent;

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

	hasPowerup1 = false;
	hasPowerup2 = false;
	hasSpeedLeft = false;
	hasSpeedRight = false;
	hasSizeLeft = false;
	hasSizeRight = false;

	bReplicates = true;
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
}

// Called to bind functionality to input
void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement 
		EnhancedInputComponent->BindAction(RightMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveRightInput);
		EnhancedInputComponent->BindAction(LeftMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveLeftInput);
		EnhancedInputComponent->BindAction(UpMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveUpInput);
		EnhancedInputComponent->BindAction(DownMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveDownInput);
		EnhancedInputComponent->BindAction(LeftPowerup, ETriggerEvent::Triggered, this, &AMyPlayerPawn::ActivateLeftPowerup);
		EnhancedInputComponent->BindAction(RightPowerup, ETriggerEvent::Triggered, this, &AMyPlayerPawn::ActivateRightPowerup);
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Move Left
void AMyPlayerPawn::MoveLeftInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Left;
	}
}








//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Move Right
void AMyPlayerPawn::MoveRightInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Right;
	}
}

// Move Up
void AMyPlayerPawn::MoveUpInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Up;
	}
}

// Move Down
void AMyPlayerPawn::MoveDownInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		CurrentDirection = EDirection::Down;
	}
}

void AMyPlayerPawn::MovePlayer()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server is handling movement."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client is calling server move."));
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
	if (Powerup == nullptr)
	{
		return;
	}

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

	if (hasPowerup1 && hasPowerup2) {
		hasMaxPowerup = true;
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
	// Check if the input button for the left power-up is pressed
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		if (hasPowerup1) {
			usePowerup1 = true; // Flag the left power-up for use
			UsePowerup();
		}
		else {
			usePowerup1 = false; // No power-up available in Slot 1
		}
	}
}

void AMyPlayerPawn::ActivateRightPowerup(const FInputActionValue& Value)
{
	// Check if the input button for the right power-up is pressed
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		if (hasPowerup2) {
			usePowerup2 = true; // Flag the right power-up for use
			UsePowerup();
		}
		else {
			usePowerup2 = false; // No power-up available in Slot 2
		}
	}
}

void AMyPlayerPawn::UsePowerup()
{
	// Handle left power-up activation
	if (usePowerup1) {
		if (hasSpeedLeft) {
			ActivateSpeedPowerup();
			RemoveLeftPowerup();
		}
		else if (hasSizeLeft) {
			ActivateSizePowerup();
			RemoveLeftPowerup();
		}
	}
	
	// Handle right power-up activation
	if (usePowerup2) {
		if (hasSpeedRight) {
			ActivateSpeedPowerup();
			RemoveRightPowerup();
		}
		else if (hasSizeRight) {
			ActivateSizePowerup();
			RemoveRightPowerup();
		}
	}
}

void AMyPlayerPawn::ActivateSpeedPowerup()
{
	// Temporarily increase player movement speed
	MoveSpeed = 10.0f;
	DelayTime = 5.0f;

	// Set a timer to deactivate the speed boost after DelayTime seconds
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandler, this, &AMyPlayerPawn::DeactivateSpeedPowerup, DelayTime, false);
}

void AMyPlayerPawn::ActivateSizePowerup()
{
	// Temporarily increase player size
	FVector Scale = FVector(0.7, 0.7, 0.3);
	StaticMeshComponent->SetWorldScale3D(Scale);
	DelayTime = 5.0f;

	// Set a timer to deactivate the size boost after DelayTime seconds
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandler, this, &AMyPlayerPawn::DeactivateSizePowerup, DelayTime, false);
}

void AMyPlayerPawn::DeactivateSpeedPowerup()
{
	// Reset player movement speed to default value
	MoveSpeed = 3.0f;
}

void AMyPlayerPawn::DeactivateSizePowerup()
{
	// Reset player size to default scale
	FVector Scale = FVector(0.3, 0.3, 0.3);
	StaticMeshComponent->SetWorldScale3D(Scale);
}

void AMyPlayerPawn::RemoveLeftPowerup()
{
	// Clear all state related to the left power-up slot
	hasPowerup1 = false;
	usePowerup1 = false;
	hasMaxPowerup = false;
	hasSpeedLeft = false;
	hasSizeLeft = false;
}

void AMyPlayerPawn::RemoveRightPowerup()
{
	// Clear all state related to the right power-up slot
	hasPowerup2 = false;
	usePowerup2 = false;
	hasMaxPowerup = false;
	hasSpeedRight = false;
	hasSizeRight = false;
}

void AMyPlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the movement direction
	DOREPLIFETIME(AMyPlayerPawn, CurrentDirection);
	DOREPLIFETIME(AMyPlayerPawn, MoveSpeed);
}