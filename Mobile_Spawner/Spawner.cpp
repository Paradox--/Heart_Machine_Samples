//-----------------------------------------------------------------------------------------------------------------
// Simnply Spawns an object in the game world for a simple mobile game.											  -
// This was the verticle slice build to test some other systems for the game concept.							  -
// Nicholas Mallonee																							  -
//-----------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------
// Includes and Libraries																						  -
//-----------------------------------------------------------------------------------------------------------------
#include "Beryl.h"
#include "Engine.h"
#include "Spawner.h"

//-----------------------------------------------------------------------------------------------------------------
// Constructors 																								  -
//-----------------------------------------------------------------------------------------------------------------
ASpawner::ASpawner()
{
	// The actor can not tick
 	PrimaryActorTick.bCanEverTick = false;
}

//-----------------------------------------------------------------------------------------------------------------
// Engine Events 																								  -
//-----------------------------------------------------------------------------------------------------------------
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	// Clear any timers that may be active. 
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Clear any timers that may be active. 
	//GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandler);

	// Clear Any Timers that are live -- Old way, removed after 4.7 
	//GetWorldTimerManager().ClearTimer(this, &ASpawner::spawnInRandomSpot);

	// Set Timer to Determine spawning -- old way 
	//GetWorldTimerManager().SetTimer(this, &ASpawner::determineSpawnPath, 0.15f, true);	

	// Set the handler -- new way
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandler, this, &ASpawner::determineSpawnPath, .15f, true);
}

void ASpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ASpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear Any Timers that are live -- new way
	GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandler);

	Super::EndPlay(EndPlayReason);
		
	// If we wanted to clear all the timers. -- safe way to remove all 
	// This would need to go above the super call
	//UWorld* const world = GetWorld();

	//if (world)
	//{
	//	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	//}	

	// destroy the array in the struct 
	spawnerInformation.destroy(); 
}

//-----------------------------------------------------------------------------------------------------------------
// Basic Spawn Methods																							  -
//-----------------------------------------------------------------------------------------------------------------
void ASpawner::determineSpawnPath()
{
	// Check the current state 
	if (spawnerInformation.getState() == ESpawnerState::Random)
	{
		// ** Debug ** //
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Random Path"));
		
		// Check the last time it was changed, if it is above 50 switch the state  
		if (spawnerInformation.getCount() >= 50)
		{
			int32 rand = FMath::RandRange(0, 10);			// Random number to choose the weighted next state 

			if (rand >= 0 && rand < 7)								// 0 through 6
			{
				spawnerInformation.setState(ESpawnerState::Trail);	// Change the state to trails
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn 
				spawnInRandomSpot();
			}
			else if (rand >= 7 && rand < 10)
			{
				spawnerInformation.setState(ESpawnerState::Random);	// Change the state to random again 
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn 
				spawnInRandomSpot();
			}
			else
			{
				spawnerInformation.setState(ESpawnerState::EveryOther);	// Change the state to everyother
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn so we can go back around
				spawnInRandomSpot();
			}
		}
		else                                                // Else we know we can check and spawn an object 
		{
			float newX = generateNewRandomX();				// Generate a new X to use 
			spawnerInformation.setSpawnCount((spawnerInformation.getCount() + 1)); // Add to the count 
			
			spawnUsingGeneratedX(newX);						// Spawn the new object 
		}
	}
	else if (spawnerInformation.getState() == ESpawnerState::Trail)
	{
		// ** Debug ** //
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Trail Path"));


		if (spawnerInformation.getCount() >= 5)
		{
			int32 rand = FMath::RandRange(0, 10);			// Random number to choose the weighted next state 

			if (rand >= 0 && rand < 7)								// 0 through 6
			{
				spawnerInformation.setState(ESpawnerState::Random);	// Change the state to trails
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn 
				spawnInRandomSpot();
			}
			else if (rand >= 7 && rand < 10)
			{
				spawnerInformation.setState(ESpawnerState::Trail);	// Change the state to random again 
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn 
				spawnInRandomSpot();
			}
			else
			{
				spawnerInformation.setState(ESpawnerState::EveryOther);	// Change the state to everyother
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn so we can go back around
				spawnInRandomSpot();
			}
		}
		else
		{
			float newX = generateTrailX();				// Generate a new X to use 
			spawnerInformation.setSpawnCount((spawnerInformation.getCount() + 1)); // Add to the count 

			spawnUsingGeneratedX(newX);						// Spawn the new object
		}
	}
	else if (spawnerInformation.getState() == ESpawnerState::EveryOther)
	{
		// ** Debug ** //
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Every Other Path"));


		if (spawnerInformation.getCount() >= 7)
		{
			int32 rand = FMath::RandRange(0, 10);			// Random number to choose the weighted next state 

			if (rand >= 0 && rand < 7)								// 0 through 6
			{
				spawnerInformation.setState(ESpawnerState::Random);	// Change the state to trails
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn 
				spawnInRandomSpot();
			}
			else if (rand >= 7 && rand < 10)
			{
				spawnerInformation.setState(ESpawnerState::Trail);	// Change the state to random again 
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn 
				spawnInRandomSpot();
			}
			else
			{
				spawnerInformation.setState(ESpawnerState::EveryOther);	// Change the state to everyother
				spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

				// Ignore the check for spaces and quick generate a place to spawn so we can go back around
				spawnInRandomSpot();
			}
		}
		else
		{
			float newX = generateEveryOther();						// Generate a new X to use 
			spawnerInformation.setSpawnCount((spawnerInformation.getCount() + 1)); // Add to the count 

			spawnUsingGeneratedX(newX);								// Spawn the new object
		}
	}
	else
	{
		spawnerInformation.setState(ESpawnerState::Random);	// Change the state to trails
		spawnerInformation.setSpawnCount(0);				// Set the spawn count back to 0 

		// Ignore the check for spaces and quick generate a place to spawn 
		spawnInRandomSpot();
	}
}

void ASpawner::spawnInRandomSpot()
{
	// Check for world and have it as a const
	UWorld* const world = GetWorld();

	// if the world exists try to spawn the object 
	if (world)
	{
		// Generate an X to use
		float x = FMath::RandRange(-270, 270);
		
		// generate to the left of the player 
		FVector loc = FVector(0.f, x, 500.f);

		// set the spawn params 
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		// Spawn the actor 
		GetWorld()->SpawnActor<AOrb>(basicOrb, loc, FRotator::ZeroRotator, spawnParams);
	}
}

void ASpawner::spawnUsingGeneratedX(float inX)
{
	// Check for world and have it as a const
	UWorld* const world = GetWorld();

	// if the world exists try to spawn the object 
	if (world)
	{
		// Package the X into a vector 
		FVector loc = FVector(0.f, inX, 500.f);

		// Save the packaged Vector
		spawnerInformation.addLocation(loc);

		// set the spawn params 
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		// Spawn the actor 
		GetWorld()->SpawnActor<AOrb>(basicOrb, loc, FRotator::ZeroRotator, spawnParams);
	}
}

//-----------------------------------------------------------------------------------------------------------------
// Basic Number Generation	and check 																			  -
//-----------------------------------------------------------------------------------------------------------------
bool ASpawner::canUseX(float inX)
{
	if (spawnerInformation.getArraySize() > 5)
	{
		if (spawnerInformation.isXInArray(inX))		// if the exact X was found return 
		{											// This Method will return false. 
													// The Method was removed after this build. 
			return false;							// Igonore this. 
		}
		else
			return true;
	}
	else
		return true;
}

float ASpawner::generateNewRandomX()
{
	return FMath::RandRange(-270.f, 270.f);
}

float ASpawner::generateTrailX()
{
	// Get the last location and the X from that
	FVector last;
	float lastX = 0;

	if (spawnerInformation.getArraySize() >= 10)
	{
		last = spawnerInformation.getLocation((9));
		lastX = last.X;
	}
	else if (spawnerInformation.getArraySize() >= 1)
	{ 
		last = spawnerInformation.getLocation((spawnerInformation.getArraySize() - 1));
		lastX = last.X;
	}
	else
	{
		lastX = 0.f; 
	}	

	if (lastX <= -260.f || lastX >= 260.f)
	{
		if (lastX <= -260.f)
		{
			lastX = lastX + 50.f;
		}
		else if (lastX >= 260.f)
		{
			lastX = lastX - 50.f; 
		}
	}
	else
	{
		int32 rand = FMath::RandRange(0, 1);

		if (rand == 0)
		{
			lastX = lastX - 40.f;
		}
		else
		{
			lastX = lastX + 40.f; 
		}
	}

	return lastX;
}

float ASpawner::generateEveryOther()
{
	// Get the last location and the X from that
	FVector last;
	float lastX = 0.f;

	if (spawnerInformation.getArraySize() >= 10)
	{
		last = spawnerInformation.getLocation((9));
		lastX = last.X;
	}
	else if (spawnerInformation.getArraySize() >= 1)
	{
		last = spawnerInformation.getLocation((spawnerInformation.getArraySize() - 1));
		lastX = last.X;
	}
	else
	{
		lastX = 150.f;
	}

	if (lastX < 0.f && lastX != 0.f)
	{
		lastX = lastX * (-1); 
	}
	else if (lastX > 0.f && lastX != 0.f)
	{
		lastX = lastX * (-1);
	}
	else
	{
		lastX = 150.f; 
	}
	
	return lastX; 
}
