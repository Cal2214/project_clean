// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDirtSpawner.h"
#include "MyDirt.h" 
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values
AMyDirtSpawner::AMyDirtSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AMyDirtSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//SpawnDirt();
	if (HasAuthority())
	{
		SpawnDirt();
	}
}

// Called every frame
void AMyDirtSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDirtSpawner::DelayDirt()
{
	SpawnTime = FMath::FRandRange(5.0, 20.0);

	GetWorld()->GetTimerManager().SetTimer(MyTimerHandler, this, &AMyDirtSpawner::SpawnDirt, SpawnTime, false);
}

void AMyDirtSpawner::SpawnDirt()
{
	//FActorSpawnParameters spawnParams;
	//spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	//if(DirtClass)
	//GetWorld()->SpawnActor<AMyDirt>(DirtClass, GetActorLocation(), FRotator::ZeroRotator, spawnParams);

	Multi_SpawnDirt();
	DelayDirt();
}

void AMyDirtSpawner::Server_DelayDirt_Implementation() 
{
	DelayDirt();
}

bool AMyDirtSpawner::Server_DelayDirt_Validate()
{
	return true;
}

void AMyDirtSpawner::Multi_SpawnDirt_Implementation()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	if (DirtClass)
	{
		// Spawn the dirt actor
		AMyDirt* SpawnedDirt = GetWorld()->SpawnActor<AMyDirt>(DirtClass, GetActorLocation(), FRotator::ZeroRotator, spawnParams);

		// Set the spawned actor to replicate
		if (SpawnedDirt)
		{
			SpawnedDirt->SetReplicates(true);
			SpawnedDirt->SetReplicateMovement(true);
		}
	}
}



