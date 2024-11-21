// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDirtSpawner.h"
#include "MyDirt.h" 
#include "TimerManager.h"

// Sets default values
AMyDirtSpawner::AMyDirtSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyDirtSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDirt();
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
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	if(DirtClass)
	GetWorld()->SpawnActor<AMyDirt>(DirtClass, GetActorLocation(), FRotator::ZeroRotator, spawnParams);

	DelayDirt();
}

