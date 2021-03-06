//-----------------------------------------------------------------------------------------------------------------
// Simnply Spawns an object in the game world for a simple mobile game.											  -
// This was the verticle slice build to test some other systems for the game concept.							  -
// Nicholas Mallonee																							  -
//-----------------------------------------------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------------------------------------------
// Includes and Libraries																						  -
//-----------------------------------------------------------------------------------------------------------------
#include "GameFramework/Actor.h"
#include "Code_Base/Orbs/Orb.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Spawner.generated.h"

//-----------------------------------------------------------------------------------------------------------------
// Spawner States	-- Enum																						  -
//-----------------------------------------------------------------------------------------------------------------
UENUM()
enum class ESpawnerState : uint8
{
	Random,
	Trail, 
	EveryOther
};

//-----------------------------------------------------------------------------------------------------------------
// Spawner Struct                                                                                                 -
//-----------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FSpawnerInformation
{
	GENERATED_USTRUCT_BODY()

	// -- Private -- Struct information 
private:
	UPROPERTY()
	ESpawnerState state;

	UPROPERTY()
	TArray<FVector> lastUsedLocations;

	UPROPERTY()
	int32 spawnCount;

	// -- Public -- Accessors, Mutators, Other methods, and constuctor
public:
	/*
	 * Takes a state to change the spawner to
	 * @Param  The state that the spawner should be changed to
	 */
	void setState(ESpawnerState inState)
	{
		state = inState;
	}
		
	/* 
	 * Adds a Vector to the struct's array 
	 * @Param The Vector in which to add to the array	
	 */
	void addLocation(FVector loc)
	{
		if (lastUsedLocations.Num() > 10)
		{
			lastUsedLocations.RemoveAt(0);
			lastUsedLocations.Add(loc);
		}
		else
			lastUsedLocations.Add(loc);	
	}

	/*
	 * Changes the number of Spawn Count
	 * @Param Number to change the spawn count to 
	 */
	void setSpawnCount(int32 in)
	{
		spawnCount = in;
	}

	/* 
	 * Returns the state of the spawners behavior
	 * @Return the current state of the spawner
	 */
	ESpawnerState getState()
	{
		return state;
	}

	/*
     * @Param Takes in a position to return 
	 * @Return The Vector at a given Position	
	 */
	FVector getLocation(int32 pos)
	{
		return lastUsedLocations[pos];	
	}
	
	/* 
	 * Returns the current count of how many times a spawn method has been called
	 * Since the last time it switched.
	 * @Return the int32 value of many times a metod was called 	
	 */
	int32 getCount()
	{
		return spawnCount;	
	}

	/* 
	 * Returns the array's logical size 
	 * @Returns the arrays size + 1
	 */
	int32 getArraySize()
	{
		return lastUsedLocations.Num();
	}

	/*
	 * Checks for an exact x in the array using a linear search 
	 * Note: This Method was never used and was later removed.  
	 * @Returns If an X was found
	 */
	bool isXInArray(float x)
	{
		bool flag = false;			

		return flag;
	}

	/* 
	 * Destroyes the objects to Gives the Memory back to the system	  
	 */
	void destroy()
	{
		lastUsedLocations.~TArray();
	}

	// -- Default Constructor
	FSpawnerInformation()
	{
		state = ESpawnerState::Random;
		spawnCount = 0;
		lastUsedLocations.SetNum(0);
	}
};

//-----------------------------------------------------------------------------------------------------------------
// Spawner Class Information																					  -
//-----------------------------------------------------------------------------------------------------------------
UCLASS()
class BERYL_API ASpawner : public AActor
{
	GENERATED_BODY()
	

	// -- Public Information -- Constructor and Engine Events -- // 
public:	
	ASpawner();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// -- Private Information -- Spawning Methods -- // 
private:
	void determineSpawnPath();				

	void spawnInRandomSpot();

	void spawnUsingGeneratedX(float inX);

	// -- Private Information -- Number Generation and Check -- // 
private:
	bool canUseX(float inX);

	float generateNewRandomX();

	float generateTrailX();

	float generateEveryOther();

	// -- Private Information -- Orbs to Spawn -- // 
private:
	UPROPERTY(EditDefaultsOnly, Category = "Basic Orb")
	TSubclassOf<AOrb> basicOrb;

	// -- Private Information -- Spawner Information Struct -- //
private:
	FSpawnerInformation spawnerInformation; 	

	// -- Private Information -- Timers -- // 
private:
	//FTimerManager spawnTimer; 

	FTimerHandle spawnTimerHandler; 
};
