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

	
}

// Called to bind functionality to input
void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement 
		EnhancedInputComponent->BindAction(RightMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveRight);
		EnhancedInputComponent->BindAction(LeftMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveLeft);
		EnhancedInputComponent->BindAction(UpMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveUp);
		EnhancedInputComponent->BindAction(DownMove, ETriggerEvent::Triggered, this, &AMyPlayerPawn::MoveDown);
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

void AMyPlayerPawn::MoveLeft(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		FVector MovementDirection;

		FVector Reverse = GetActorRightVector() * -1.0;

		MovementDirection.Y = GetActorLocation().Y + (Reverse.Y * MoveSpeed);
		MovementDirection.X = GetActorLocation().X;
		MovementDirection.Z = GetActorLocation().Z;

		SetActorLocation(MovementDirection, false, nullptr, ETeleportType::None);
	}
}

void AMyPlayerPawn::MoveRight(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		FVector MovementDirection;
		MovementDirection.Y = GetActorLocation().Y + (GetActorRightVector().Y * MoveSpeed);
		MovementDirection.X = GetActorLocation().X;
		MovementDirection.Z = GetActorLocation().Z;
		
		SetActorLocation(MovementDirection, false, nullptr, ETeleportType::None);
	}
}

void AMyPlayerPawn::MoveUp(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		FVector MovementDirection;
		MovementDirection.X = GetActorLocation().X + (GetActorForwardVector().X * MoveSpeed);
		MovementDirection.Y = GetActorLocation().Y;
		MovementDirection.Z = GetActorLocation().Z;

		SetActorLocation(MovementDirection, false, nullptr, ETeleportType::None);
	}
}

void AMyPlayerPawn::MoveDown(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (CurrentValue)
	{
		FVector MovementDirection;

		FVector Reverse = GetActorForwardVector() * -1.0;

		MovementDirection.X = GetActorLocation().X + (Reverse.X * MoveSpeed);
		MovementDirection.Y = GetActorLocation().Y;
		MovementDirection.Z = GetActorLocation().Z;

		SetActorLocation(MovementDirection, false, nullptr, ETeleportType::None);
	}
}