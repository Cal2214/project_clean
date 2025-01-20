// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDirt.h"

// Sets default values
AMyDirt::AMyDirt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderComponent"));
	BoxCollider->SetupAttachment(RootComponent);

	Points = 10;
}

// Called when the game starts or when spawned
void AMyDirt::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyDirt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDirt::CollectDirt()
{
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Destroy();
}

