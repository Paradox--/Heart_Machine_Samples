// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Procedural_Forest_Spawner.generated.h"


USTRUCT(BlueprintType)
struct FSpawnPoint
{
	GENERATED_USTRUCT_BODY()

private:
	FVector loc;
	FVector pointBeforeVarience; 
	FVector lastLoc;
	float xVarience;
	float yVarience; 

public:
	FVector getLoc()
	{
		return loc;
	}

	FVector getPointBeforeVarience()
	{
		return pointBeforeVarience;
	}

	FVector getLastLoc()
	{
		return lastLoc;
	}

	float getXVarience()
	{
		return xVarience;
	}

	float getYVarience()
	{
		return yVarience; 
	}

	void updateXVarience(float x)
	{
		xVarience = x;
	}

	void updateYVarience(float y)
	{
		yVarience = y;
	}

	void updateLocation(FVector location)
	{
		loc = location; 
	}

	void updatePointBeforeVarience(FVector location)
	{
		pointBeforeVarience = location;
	}

	void updateLastLocation(FVector location)
	{
		lastLoc = location; 
	}

	FSpawnPoint()
	{
		loc = FVector(0.f, 0.f, 0.f);
		pointBeforeVarience = FVector(0.f, 0.f, 0.f);
		lastLoc = FVector(0.f, 0.f, 0.f);
		xVarience = 0.f; 
		yVarience = 0.f;
	}
};

UCLASS()
class AI_PLAYGROUND_API AProcedural_Forest_Spawner : public AActor
{
	GENERATED_BODY()
	
	// -- Public Information -- Construstor and Engine Events -- //
public:	
	AProcedural_Forest_Spawner();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;	

	// -- Private Information -- Preferred Type and distance between trees --// 
private:
	int32 preferredTreeType = 0;

	int32 preferredShrubType = 0; 

	int32 countTillNextWeightedType = 1; 
	
	float distanceBetweenTrees = 0.f;

	float distanceBetweenShrubs = 0.f;

	float spawnRadiusOfShrubs = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	int32 min_CountTillNextWeightedType = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	int32 max_CountTillNextWeightedType = 100; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	int32 min_NumOfShrubs = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	int32 max_NumOfShrubs = 5; 

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Distances")
	float min_distanceBetweenTrees = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Distances")
	float max_distanceBetweenTrees = 2500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Distances")
	float min_distanceBetweenShrubs = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Distances")
	float max_distanceBetweenShrubs = 500.f;
		
	// -- Private Information -- Spawn Params -- // 
private:
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float preferredTypeChance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float min_XAxis = -1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float max_XAxis = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float min_YAxis = -1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float max_YAxis = 1000.f; 

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float lineHeight = 1000.f; 

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float min_VarienceInX = -100.f; 

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float max_VarienceInX = 100.f; 

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float min_VarienceInY = -100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Params")
	float max_VarienceInY = 100.f;
	
	float currentRowIn_Y = 0.f; 

	float currentColIn_X = 0.f; 

	FVector currentPointInSpace;

	FVector oldPointInSpace; 

	FSpawnPoint currentSpawnLocationInformation; 

	// -- Private Information -- Step Checker -- // 
private:
	bool bHadToStep = false;

	bool bNextStepWillMiss = false; 

	// -- Private Information -- Debugging Helpers -- //
private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug Suite")
	bool bWantDebugLines = false; 

	UPROPERTY(EditDefaultsOnly, Category = "Debug Suite")
	bool bWantDebugText = false; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug Suite")
	float additionDebugLineTime = .1f; 
	
	// -- Private Information -- Timer Handle -- // 
private:
	FTimerHandle spawnTimerHandle; 

	UPROPERTY(EditDefaultsOnly, Category = "Spawner Iteration Timer")
	float timerIterationTime = 1.f;

	// -- Public Information -- Timer Handle Related Methods -- //
public:
	void setUpTimer();

	void clearTimer(); 

	// -- Private Information -- Tree Types To Spawn -- //
private:
	UPROPERTY(EditDefaultsOnly, Category = "Tree Types")
	TSubclassOf<AActor>  tree1;

	UPROPERTY(EditDefaultsOnly, Category = "Tree Types")
	TSubclassOf<AActor>  tree2;

	UPROPERTY(EditDefaultsOnly, Category = "Tree Types")
	TSubclassOf<AActor>  tree3;

	UPROPERTY(EditDefaultsOnly, Category = "Tree Types")
	TSubclassOf<AActor>  tree4;

	UPROPERTY(EditDefaultsOnly, Category = "Tree Types")
	TSubclassOf<AActor>  tree5;

	// -- Private Information -- Shrubs Types To Spawn -- // 
private:
	UPROPERTY(EditDefaultsOnly, Category = "Shrub Types")
	TSubclassOf<AActor>  shrub1;

	UPROPERTY(EditDefaultsOnly, Category = "Shrub Types")
	TSubclassOf<AActor>  shrub2;

	UPROPERTY(EditDefaultsOnly, Category = "Shrub Types")
	TSubclassOf<AActor>  shrub3;

	UPROPERTY(EditDefaultsOnly, Category = "Shrub Types")
	TSubclassOf<AActor>  shrub4;

	UPROPERTY(EditDefaultsOnly, Category = "Shrub Types")
	TSubclassOf<AActor>  shrub5;

	// -- Public Information --  Spawn Methods -- // 
public:
	void bootSpawner(); 

	void updateRowAndColumns(); 
	
	void spawnObject(); 

	void spawnTree(FVector pointToUse, int32 type = 0);

	void spawnShrub(FVector pointToUse);

	bool isTheEnd();

	bool canUseX(float x);

	bool canUseY(float y);

	int32 chooseTreeToSpawn();

	int32 chooseShrubType();

	FVector generatePoint(); 

	FVector nextSuggestedPoint();

	FVector chooseShrubPoint(FVector location);

	FVector findLanding(FVector oldpoint);
	
// -- Private Information -- Height Related Service -- // 
private:
	bool checkNextStep(FVector pointUsed); 

	bool isValidPoint(FVector location); 

	bool validPoint(FHitResult &hit, FVector origin, FVector traceDir);

	bool worldTrace(FHitResult &hit, FVector startPoint, FVector endPoint);

	FVector doTrace(FVector location);

	FVector getImpacePoint(FHitResult &hit, FVector origin, FVector traceDir);
};
