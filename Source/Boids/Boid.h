// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"


UCLASS()
class BOIDS_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	class ABoidManager* Manager;
	FVector currentVelocity = FVector::ZeroVector;

	
	//UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Settings") //redundant
	//bool isRepulsive = true; 
	//UPROPERTY(Config, EditAnywhere, BlueprintReadWrite) //redundant
	//FVector Force;

	//Object avoidance
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	TArray<FHitResult> Hits;
	//Object avoidance
	//timers
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	float wanderTimer = 0.f;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	float FleeTimer = 0.f;
	//timers
	


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UStaticMeshComponent* Mesh;
	
	//Types Of Movement 
	UFUNCTION(BlueprintCallable)
	FVector Seek(FVector Position);
	UFUNCTION(BlueprintCallable)
	FVector Flee(FVector Position);
	FVector Wander(float Radius, float Distance, float Jitter);
	FVector WanderDestination;
	FVector Alignment(TArray<ABoid*> Neighbors);
	FVector Seperation(TArray<ABoid*> Neighbors);
	FVector Cohesion(TArray<ABoid*> Neighbors);
	//Types of movement

	FVector Boundries(FVector CurrentVel, FVector CentrePoint, float Radius, float Threshold);
	FVector AvoidObj();

	
	//boundaries


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoid(float Deltatime);
	
	APlayerController* PlayerController; //player
};
