// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this -> SetRootComponent(transform);

}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	

}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ABoid* Boid : MyBoids) { 
		Boid->UpdateBoid(DeltaTime); //boids are updated in manager for optimization
	}
	if (DoBorder) {
		ChangeBorder();
	}
	

}

TArray<class ABoid*> ABoidManager::GetBoidFamily(ABoid* ThisBoid)
{


	TArray<class ABoid*> ReturnBoids;


	for (ABoid* Boid : MyBoids) {
		if (Boid == ThisBoid || !Boid)     continue; //list of boids and distances
		float aDistance = (Boid->GetActorLocation() - ThisBoid->GetActorLocation()).Size();
		if (aDistance < FamilyRadius)     ReturnBoids.Add(Boid);
				

	}
	return ReturnBoids;
}

void ABoidManager::ChangeBorder()
{
	DrawDebugSphere(GetWorld(), FVector::ZeroVector, BoundaryRadius, 18, FColor::Black, false, -1.f, 0U, 5.f); //border being drawn through slider
}


void ABoidManager::SpawnBoids(int SpawnCounter)
{
	MyBoids.Reset();
	
	for (int i = 0; i < SpawnCount; i++) {
		FVector SpawnLocation = (FMath::VRand() * FMath::RandRange(0.0f, SpawnRadius)) + GetActorLocation(); //spawning boids in a random range
		FRotator SpawnRotation = GetActorRotation();

		ABoid* Newboid = GetWorld()->SpawnActor<ABoid>(BoidBP, SpawnLocation, SpawnRotation); //spawns the blueprint version of the boid so that rotation can be correct
		Newboid->Manager = this;
		MyBoids.Add(Newboid); //adds the boid to the boid list and object avoidance ignores
		ActorsToignore.Add(Newboid);
	}

}




