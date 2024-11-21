// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPowerup.h"

// Sets default values
AMyPowerup::AMyPowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderComponent"));
	BoxCollider->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyPowerup::BeginPlay()
{
	Super::BeginPlay();
	
	GetPowerup();
}

// Called every frame
void AMyPowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EPowerupType AMyPowerup::GetPowerup()
{
	int RandomValue = rand() % 2;

	switch (RandomValue)
	{
	case 0:
		CurrentPowerup = EPowerupType::Speed;
		break;
	case 1:
		CurrentPowerup = EPowerupType::Size;
		break;
	default:
		break;
	}
	UE_LOG(LogTemp, Log, TEXT("Your message: %d"), CurrentPowerup);
	return CurrentPowerup;
}

