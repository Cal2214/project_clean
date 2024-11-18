// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerPawn.h"

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

void AMyPlayerPawn::MoveLeftInput(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		CurrentDirection = EDirection::Left;
	}
}

void AMyPlayerPawn::MoveRightInput(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		CurrentDirection = EDirection::Right;
	}
}

void AMyPlayerPawn::MoveUpInput(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		CurrentDirection = EDirection::Up;
	}
}

void AMyPlayerPawn::MoveDownInput(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		CurrentDirection = EDirection::Down;
	}
}

void AMyPlayerPawn::MovePlayer()
{
	this->MovementDirection = GetActorLocation(); // Initialize MovementDirection with current location
	FVector Reverse = FVector::ZeroVector;        // Initialize Reverse vector

	switch (CurrentDirection)
	{
	case EDirection::Right:
		this->MovementDirection.Y = GetActorLocation().Y + (GetActorRightVector().Y * MoveSpeed);
		break;

	case EDirection::Left:
		Reverse = GetActorRightVector() * -1.0;
		this->MovementDirection.Y = GetActorLocation().Y + (Reverse.Y * MoveSpeed);
		break;

	case EDirection::Up:
		this->MovementDirection.X = GetActorLocation().X + (GetActorForwardVector().X * MoveSpeed);
		break;

	case EDirection::Down:
		Reverse = GetActorForwardVector() * -1.0;
		this->MovementDirection.X = GetActorLocation().X + (Reverse.X * MoveSpeed);
		break;

	default:
		break;
	}

	// Call SetActorLocation outside the switch to ensure it executes for all cases
	SetActorLocation(this->MovementDirection, false, nullptr, ETeleportType::None);
}

void AMyPlayerPawn::AddPoints(int32 Ammount)
{
	PlayerScore += Ammount; 
}

int32 AMyPlayerPawn::GetPoints()
{
	return PlayerScore;
}