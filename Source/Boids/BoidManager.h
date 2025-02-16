// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

UCLASS()
class BOIDS_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	TSubclassOf<class ABoid> BoidBP;
	//Spawn Settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	int SpawnCount = 30;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	float SpawnRadius = 500.f;
	//Spawn Settings


	//Boid Movement
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "BoidSettings")
	float SpeedMultiplier = 30;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "BoidSettings")
	float FamilyRadius = 500.f;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "BoidSettings")
	float SeparationMultliplier = 1.f;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "BoidSettings")
	float CohesionMultliplier = 1.f;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "BoidSettings")
	float AllignmentMultliplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WanderSettings")
	TArray<class ABoid*> MyBoids;
	//BoidMovement


	//Mouse Influence
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MouseSettings")
	bool LeftIsPressed;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MouseSettings")
	bool RightIsPressed;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "MouseSettings")
	FVector MousePosition;
	//Mouse Influence
	
	

	//Boundaries
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "BoundarySettings")
	float BoundaryRadius;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "BoundarySettings")
	float BoundaryThreshold;


	//Boid Variables
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "WanderSettings")
	float WanderDistance = 500.0f;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "WanderSettings")
	float WanderRadius = 500.0f;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "WanderSettings")
	float JitterMultiplier = 300.0f;
	//Boid Variables

	//Debug
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	bool FamilyradiusCheck = false;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	bool DoBorder = false;

	

	
	

	TArray<AActor*> ActorsToignore;
	USceneComponent* transform;

	

protected:
	// Called when the game starts or when spawned
	UFUNCTION(BlueprintCallable)
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<class ABoid*> GetBoidFamily(ABoid* ThisBoid); //families

	UFUNCTION(BlueprintCallable)
	void ChangeBorder(); //border size on slider

	

	UFUNCTION(BlueprintCallable)
	void SpawnBoids(int SpawnCounter); //spawner

};
