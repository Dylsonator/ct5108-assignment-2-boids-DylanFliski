// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "BoidManager.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	 PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); //gets player controlls for mouse click 

	}


//FLOCKING VARIATIONS
FVector ABoid::Seek(FVector Position)
{
	FVector newVelocity = Position - GetActorLocation();
	newVelocity.Normalize();
	 //velocity is set to the position
	return newVelocity;
}

FVector ABoid::Flee(FVector Position)
{
	FVector newVelocity = GetActorLocation() - Position;
	newVelocity.Normalize();
	 //velocity oposite of the loation of the position
	return newVelocity;
}

FVector ABoid::Wander(float Radius, float Distance, float Jitter)
{
	FVector Location = GetActorLocation();
	if (FVector::Dist(Location, WanderDestination) < 100.0f || wanderTimer <= 1) //makes sure it keeps changing the wander so that it doesnt try to get places it cant
	{
		FVector ProjectedPos = Location + (GetActorForwardVector() * Distance);
		WanderDestination = ProjectedPos + (FMath::VRand() * FMath::RandRange(0.0f, Jitter));
		wanderTimer = 0; //finds a new location in a range infront of it
	}

	FVector JitterDestination = Seek(WanderDestination) + (FMath::VRand() * FMath::RandRange(0.0f, Jitter));
	return JitterDestination; // seeks towards the wander destination
}

FVector ABoid::Alignment(TArray<ABoid*> Neighbors)
{
	if (Neighbors.Num() == 0) return FVector::ZeroVector;
	FVector newVelocity; //nothing happens

	for (ABoid* Boid : Neighbors) // trys to even out the seperation and cohesion
	{
		newVelocity += Boid->currentVelocity;
	}

	newVelocity /= Neighbors.Num();
	newVelocity.Normalize();

	return newVelocity;
}


FVector ABoid::Seperation(TArray<ABoid*> Neighbors)
{
	if (Neighbors.Num() == 0) return FVector::ZeroVector;
	FVector newFlee; //if 0 nothing happens


	for (ABoid* Boid : Neighbors) //stay as evenly away from other boids as possible
	{
		newFlee += Flee(Boid->GetActorLocation()); //run from other boids
	}
	newFlee.Normalize(); 

	return newFlee;
}


FVector ABoid::Cohesion(TArray<ABoid*> Neighbors) //cohesion function
{
	if (Neighbors.Num() == 0) return FVector::ZeroVector;
	FVector newLocation; //if the number of neigbors are 0 nothing happens

	float inverseValue = 1 / Neighbors.Num();

	for (ABoid* Boid : Neighbors)
	{ //get as close to the other boids as possible
		newLocation += (Boid->GetActorLocation() * inverseValue);
	}

	return Seek(newLocation);
}

FVector ABoid::Boundries(FVector CurrentVel, FVector CentrePoint, float Radius, float Threshold) //Boundary for the BOIDS
{
	FVector Centre = CentrePoint - GetActorLocation(); //setups
	float ToCentre = Centre.Size();

	if (ToCentre > Radius) { //if the distance to centre is larger than the radius go back towards
		FVector CorrectionForce = Centre.GetSafeNormal() * (ToCentre - Radius);
		CurrentVel += CorrectionForce;
	}
	else if (Radius - ToCentre < Threshold) { //else try stay within the threshold of the sphere
		FVector AvoidEdge = Centre.GetSafeNormal() * (Threshold - (Radius - ToCentre));
		CurrentVel += AvoidEdge;
	}
	return CurrentVel.GetClampedToMaxSize(Manager->SpeedMultiplier);

}


FVector ABoid::AvoidObj()
{	
	

	bool hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
	GetActorLocation() + currentVelocity * 200, 120,
	UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),
	true, Manager->ActorsToignore, EDrawDebugTrace::None, Hits, true);
	//creates a capsule from the boid to infront of it detecting if it hits something

	if (hit && FleeTimer)
	{//if it hits and it hasnt hit too soon it flees from the obsticle
		float Distance = FVector::Distance(GetActorLocation(), Hits[0].Location);
		return Flee(Hits[0].Location * Distance);
		
	}
	
	else {
		return currentVelocity;
		
	}
	
}

//FVector ABoid::MouseInfluence(TArray<ABoid*> Neighbors, ABoid* Boid) /////Old Mouse Influence Code (Redundant)/////
//{
//	FVector Location;
//	FVector Direction;	
//
//	
//	FVector Displacement = Manager->MousePosition - Boid->GetActorLocation();/////Old Mouse Influence Code (Redundant)/////
//	float distance = FVector::Distance(GetActorLocation(), Location);
//
//	Force = Displacement / exp(distance / 100.0f);
//	if (isRepulsive)
//	{
//		Force *= 1.0f;		
//		Force.Normalize();/////Old Mouse Influence Code (Redundant)/////
//		return Force;
//	}
//	else
//	{
//		return FVector();/////Old Mouse Influence Code (Redundant)/////
//	}
//}
//END OF FLOCKING




// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (wanderTimer < 0) {
		wanderTimer += DeltaTime; //timers for reseting wander and flee
	}
	if (FleeTimer < 0) {
		wanderTimer += DeltaTime;
	}
	///debug
	if (Manager->FamilyradiusCheck) {
		DrawDebugSphere(GetWorld(), GetActorLocation(), Manager->FamilyRadius, 18, FColor::Black, false, -1.f, 0, 1.f); //border being drawn through slider
	}

}

void ABoid::UpdateBoid(float DeltaTime)
{
	TArray<ABoid*> closestBoid = Manager->GetBoidFamily(this);
	FVector targetVelocity = FVector::ZeroVector;	
	targetVelocity += AvoidObj(); //avoid object has highest priority
	
	////////////////applying Velocity
	if (Manager->LeftIsPressed) {
		targetVelocity = Seek(Manager->MousePosition) * 100; // x100 for weighted 
	}
	 //seek the mouse click or run away from it
	if (Manager->RightIsPressed) {
		targetVelocity = Flee(Manager->MousePosition);
	}
	///	

		//steering behaviours
		targetVelocity += Seperation(closestBoid) * Manager->SeparationMultliplier; //seperates the boids first after influences

		if (targetVelocity.Size() < 0.6f) {
			targetVelocity += Cohesion(closestBoid) * Manager->CohesionMultliplier;
		}
		if (targetVelocity.Size() < 1.f) {

			targetVelocity += Alignment(closestBoid) * Manager->AllignmentMultliplier; //allignment and cohesion are similar to allow a nice flock
		}	
		
		
			
	targetVelocity.Normalize();

	targetVelocity = Boundries(targetVelocity, FVector::ZeroVector, Manager->BoundaryRadius, Manager->BoundaryThreshold); //boundaries
	//apply force info
	FVector NewForce = targetVelocity - currentVelocity;  //sets the current velocity of the boid


	currentVelocity += NewForce * DeltaTime; //moves the boid through deltatime		

	FVector location = GetActorLocation();
	location += (currentVelocity * Manager->SpeedMultiplier * DeltaTime);
	
	
	FVector Direction = currentVelocity.GetSafeNormal();
	SetActorRotation(Direction.Rotation()); //get the location of the boids movement and sets it to a rotation

	SetActorLocation(location); //sets location of the boid

}



