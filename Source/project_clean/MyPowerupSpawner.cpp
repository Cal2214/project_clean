// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPowerupSpawner.h"

#include "MyPowerup.h"
#include "NavigationSystem.h"


// Sets default values
AMyPowerupSpawner::AMyPowerupSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPowerupSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	DelayPowerup();
}

// Called every frame
void AMyPowerupSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPowerupSpawner::DelayPowerup()
{
	SpawnTime = FMath::FRandRange(5.0, 20.0);

	GetWorld()->GetTimerManager().SetTimer(MyTimerHandler, this, &AMyPowerupSpawner::SpawnPowerup, SpawnTime, false);
}

void AMyPowerupSpawner::SpawnPowerup()
{
	FVector SpawnOrigin = GetActorLocation();
	float SpawnRadius = 7000.0f;
	FVector RandomPoint = SpawnOrigin;

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem) {
		FNavLocation NavLocation;
		if (NavSystem->GetRandomReachablePointInRadius(SpawnOrigin, SpawnRadius, NavLocation)) {
			RandomPoint = NavLocation.Location;
		}
	}

	RandomPoint.Z = 2000.0f;

	if (PowerupClass)
		GetWorld()->SpawnActor<AMyPowerup>(PowerupClass, RandomPoint, FRotator::ZeroRotator);

	DelayPowerup();
}

