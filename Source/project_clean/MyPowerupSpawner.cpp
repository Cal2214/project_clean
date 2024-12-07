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
	// Generate a random number to decide the time to spawn next powerup 
	SpawnTime = FMath::FRandRange(5.0, 20.0);

	// Sets the timer to call the spawn function 
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandler, this, &AMyPowerupSpawner::SpawnPowerup, SpawnTime, false);
}

void AMyPowerupSpawner::SpawnPowerup()
{
	FVector SpawnOrigin = GetActorLocation(); // Get the spawner's origin point to use as the center for randomization
	float SpawnRadius = 7000.0f;		      // Define the radius within which power-ups can spawn
	FVector RandomPoint = SpawnOrigin;		  // Default spawn point is the spawner's origin

	// Use Unreal's Navigation System to find a random, reachable point within the radius
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (NavSystem) {
		FNavLocation NavLocation;
		// Get a random, valid point within the defined radius
		if (NavSystem->GetRandomReachablePointInRadius(SpawnOrigin, SpawnRadius, NavLocation)) {
			RandomPoint = NavLocation.Location;
		}
	}
	// Set the Z-axis to ensure the power-up spawns high enough to fall to the ground
	RandomPoint.Z = 2000.0f;

	// Spawn the power-up actor at the calculated location, if the PowerupClass is valid
	if (PowerupClass)
		GetWorld()->SpawnActor<AMyPowerup>(PowerupClass, RandomPoint, FRotator::ZeroRotator);

	// Call DelayPowerup to reset the timer for the next spawn
	DelayPowerup();
}

