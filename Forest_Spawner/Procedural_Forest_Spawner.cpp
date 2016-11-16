// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Playground.h"
#include "Engine.h"
#include "Procedural_Forest_Spawner.h"


//-------------------------------------------------------------------------------------------------------------------
// Constructor																										-
//-------------------------------------------------------------------------------------------------------------------
AProcedural_Forest_Spawner::AProcedural_Forest_Spawner()
{
	// default information 
 	PrimaryActorTick.bCanEverTick = true;
}

//-------------------------------------------------------------------------------------------------------------------
// Engine Events																									-
//-------------------------------------------------------------------------------------------------------------------
void AProcedural_Forest_Spawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const world = GetWorld();

	if (world)
	{
		bootSpawner();

		doTrace(currentPointInSpace);
	}
}

void AProcedural_Forest_Spawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AProcedural_Forest_Spawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	clearTimer();

	Super::EndPlay(EndPlayReason);
}

//-------------------------------------------------------------------------------------------------------------------
// Timer Related Methods																							-
//-------------------------------------------------------------------------------------------------------------------
void AProcedural_Forest_Spawner::setUpTimer()
{
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AProcedural_Forest_Spawner::spawnObject, timerIterationTime, true);
}

void AProcedural_Forest_Spawner::clearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandle);
}

//-------------------------------------------------------------------------------------------------------------------
// Spawner Related Methods																							-
//-------------------------------------------------------------------------------------------------------------------
void AProcedural_Forest_Spawner::bootSpawner()
{
	UWorld* const world = GetWorld();

	if (world)
	{
		// Set the preferred type
		preferredTreeType = FMath::RandRange(0, 5);
		preferredShrubType = FMath::RandRange(0, 5);

		// set the first distances 
		distanceBetweenTrees = FMath::FRandRange(min_distanceBetweenTrees, max_distanceBetweenTrees);
		distanceBetweenShrubs = FMath::FRandRange(min_distanceBetweenShrubs, max_distanceBetweenShrubs);

		// set the time till we want to try and change the next weighted type
		countTillNextWeightedType = FMath::RandRange(min_CountTillNextWeightedType, max_CountTillNextWeightedType);
		
		// set the location
		this->SetActorLocation(FVector(0.f, 0.f, lineHeight)); 
		currentPointInSpace = FVector(min_XAxis, max_YAxis, lineHeight);
		oldPointInSpace = FVector(0.f, 0.f, 0.f);

		// populate the struct 
		currentSpawnLocationInformation = FSpawnPoint(); 
		currentSpawnLocationInformation.updateLocation(currentPointInSpace);
		currentSpawnLocationInformation.updateLastLocation(currentPointInSpace);
		currentSpawnLocationInformation.updatePointBeforeVarience(currentPointInSpace);
		currentSpawnLocationInformation.updateXVarience(0.f);
		currentSpawnLocationInformation.updateYVarience(0.f);

		// Set the step keeper
		currentRowIn_Y = max_YAxis;
		currentColIn_X = min_XAxis;

		// set up the timer
		setUpTimer(); 
	}
}

void AProcedural_Forest_Spawner::updateRowAndColumns()
{
	currentRowIn_Y = currentRowIn_Y - (max_distanceBetweenTrees - min_distanceBetweenTrees); 


	currentSpawnLocationInformation.updatePointBeforeVarience(FVector(min_XAxis, currentRowIn_Y, lineHeight));
	currentSpawnLocationInformation.updateLocation(FVector(min_XAxis, currentRowIn_Y, lineHeight));
	currentSpawnLocationInformation.updateLastLocation(FVector(min_XAxis, currentRowIn_Y, lineHeight));
	currentSpawnLocationInformation.updateXVarience(0.f);
	currentSpawnLocationInformation.updateYVarience(0.f);
}

void AProcedural_Forest_Spawner::spawnObject()
{
	if (!isTheEnd())
	{
		FVector spawnPoint = generatePoint(); 
		int32 typeOfTreeToSpawn = chooseTreeToSpawn();
		//checkNextStep(spawnPoint);

		UWorld* const world = GetWorld();

		if (world)
		{
			if (!bHadToStep)
				spawnTree(spawnPoint, typeOfTreeToSpawn);
			else
			{
				bNextStepWillMiss = false;
				bHadToStep = false;
			}
				
			spawnShrub(spawnPoint); 
		}
	}
	else
		clearTimer(); 
}

void AProcedural_Forest_Spawner::spawnTree(FVector pointToUse, int32 type)
{
	// set the spawn params 
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	if (type == 0)
		GetWorld()->SpawnActor<AActor>(tree1, pointToUse, FRotator::ZeroRotator, spawnParams);
	else if (type == 1)
		GetWorld()->SpawnActor<AActor>(tree2, pointToUse, FRotator::ZeroRotator, spawnParams);
	else if (type == 2)
		GetWorld()->SpawnActor<AActor>(tree3, pointToUse, FRotator::ZeroRotator, spawnParams);
	else if (type == 3)
		GetWorld()->SpawnActor<AActor>(tree4, pointToUse, FRotator::ZeroRotator, spawnParams);
	else if (type == 4)
		GetWorld()->SpawnActor<AActor>(tree5, pointToUse, FRotator::ZeroRotator, spawnParams);
}

void AProcedural_Forest_Spawner::spawnShrub(FVector pointToUse)
{
	// set the spawn params 
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	int32 numToSpawn = FMath::RandRange(min_NumOfShrubs, max_NumOfShrubs);

	if (numToSpawn != 0)
	{
		for (int32 i = 0; i < numToSpawn; i++)
		{
			int32 type = chooseShrubType(); 

			FVector point = chooseShrubPoint(pointToUse); 

			if (type == 0)
				GetWorld()->SpawnActor<AActor>(shrub1, point, FRotator::ZeroRotator, spawnParams);
			else if (type == 1)
				GetWorld()->SpawnActor<AActor>(shrub2, point, FRotator::ZeroRotator, spawnParams);
			else if (type == 2)
				GetWorld()->SpawnActor<AActor>(shrub3, point, FRotator::ZeroRotator, spawnParams);
			else if (type == 3)
				GetWorld()->SpawnActor<AActor>(shrub4, point, FRotator::ZeroRotator, spawnParams);
			else if (type == 4)
				GetWorld()->SpawnActor<AActor>(shrub5, point, FRotator::ZeroRotator, spawnParams);
		}
	}
}

bool AProcedural_Forest_Spawner::isTheEnd()
{
	if ((currentRowIn_Y - min_VarienceInY) < min_YAxis)
		return true;
	else
		return false; 
}

bool AProcedural_Forest_Spawner::canUseX(float x)
{
	if (x >= min_XAxis && x <= max_XAxis)
		return true;
	else
		return false; 
}

bool AProcedural_Forest_Spawner::canUseY(float y)
{
	if (y >= min_YAxis && y <= max_YAxis)
		return true;
	else
		return false;
}

int32 AProcedural_Forest_Spawner::chooseTreeToSpawn()
{
	if (countTillNextWeightedType > 0)
	{
		countTillNextWeightedType--; 


		float roll = FMath::FRandRange(0.f, 100.f);

		if (roll >= preferredTypeChance)
		{
			return preferredTreeType;
		}
		else
		{
			int32 miniRoll = FMath::RandRange(0, 5);

			if (miniRoll == 0)
				return 0;
			else if (miniRoll == 1)
				return 1;
			else if (miniRoll == 2)
				return 2;
			else if (miniRoll == 3)
				return 3;
			else if (miniRoll == 4)
				return 4;
			else
				return 5; 
		}		
	}
	else
	{
		countTillNextWeightedType = FMath::RandRange(min_CountTillNextWeightedType, max_CountTillNextWeightedType);

		float roll = FMath::FRandRange(0.f, 100.f);

		if (roll >= preferredTypeChance)
		{
			preferredTreeType = preferredTreeType;
			preferredShrubType = preferredShrubType; 
			return preferredTreeType; 
		}
		else
		{
			preferredTreeType = FMath::RandRange(0, 5);
			preferredShrubType = FMath::RandRange(0, 5);
			return preferredTreeType;
		}
	}

}

int32 AProcedural_Forest_Spawner::chooseShrubType()
{
	
	float roll = FMath::FRandRange(0.f, 100.f);

	if (roll >= preferredTypeChance)
	{
		return preferredShrubType;
	}
	else
	{
		int32 miniRoll = FMath::RandRange(0, 5);

		if (miniRoll == 0)
			return 0;
		else if (miniRoll == 1)
			return 1;
		else if (miniRoll == 2)
			return 2;
		else if (miniRoll == 3)
			return 3;
		else if (miniRoll == 4)
			return 4;
		else
			return 5;
	}

}

FVector AProcedural_Forest_Spawner::generatePoint()
{
	FVector suggestedPoint = nextSuggestedPoint(); 
	
	return suggestedPoint;
}

FVector AProcedural_Forest_Spawner::nextSuggestedPoint()
{
	FVector packagedPoint = FVector(0.f, 0.f, 0.f);
	FVector pointBeforeVar = currentSpawnLocationInformation.getPointBeforeVarience(); 

	float xVarience = FMath::FRandRange(min_VarienceInX, max_VarienceInX);
	float yVarience = FMath::FRandRange(min_VarienceInY, max_VarienceInY);
	float generatedX = 0;
	float generatedY = currentRowIn_Y;
	float dist = FMath::FRandRange(min_distanceBetweenTrees, max_distanceBetweenTrees);
		
	if ( dist <= 0)
	{
		dist = dist * -1; 
	}

	generatedX = pointBeforeVar.X + dist;
	
	if (canUseX(generatedX))
	{
		// **
		// could add a check for y but not in this demo 
		// ** 
		
		// -- Try the point in space before packaging it 
		currentPointInSpace = FVector((generatedX + xVarience), (generatedY + yVarience), lineHeight);
		
		if (isValidPoint(currentPointInSpace))
		{
			FVector spawnPoint = doTrace(currentPointInSpace);

			// -- if we can use it, reset the information to include the points
			currentSpawnLocationInformation.updateLastLocation(currentSpawnLocationInformation.getLoc());
			currentSpawnLocationInformation.updatePointBeforeVarience(FVector(generatedX, currentRowIn_Y, lineHeight));
			currentSpawnLocationInformation.updateXVarience(xVarience);
			currentSpawnLocationInformation.updateYVarience(yVarience);

			generatedX += xVarience;
			generatedY += yVarience;

			packagedPoint = spawnPoint; 

			// update the last current point
			currentSpawnLocationInformation.updateLocation(packagedPoint);

			return packagedPoint;
		}
		else
		{
			bHadToStep = true; 

			currentPointInSpace.Y = currentRowIn_Y;
			currentPointInSpace = findLanding(currentPointInSpace);

			// -- if we can use it, reset the information to include the points
			currentSpawnLocationInformation.updateLastLocation(currentSpawnLocationInformation.getLoc());
			currentSpawnLocationInformation.updatePointBeforeVarience(currentPointInSpace);
			currentSpawnLocationInformation.updateXVarience(0);
			currentSpawnLocationInformation.updateYVarience(0);
			packagedPoint = currentPointInSpace;

			// update the last current point
			currentSpawnLocationInformation.updateLocation(packagedPoint);

			return packagedPoint;
		}
	}
	else
	{
		if (!isTheEnd())
		{
			updateRowAndColumns();

			return nextSuggestedPoint();
		}
		else
		{
			return FVector(0.f, 0.f, 0.f);
		}		
	}
}

FVector AProcedural_Forest_Spawner::chooseShrubPoint(FVector location)
{
	FVector loc = location;

	float x;
	float y;
	int32 randNum = FMath::RandRange(0, 1);

	if (randNum == 1)
	{
		x = loc.X + FMath::FRandRange(min_distanceBetweenShrubs, max_distanceBetweenShrubs);
		y = loc.Y + FMath::FRandRange(min_distanceBetweenShrubs, max_distanceBetweenShrubs);
	}
	else
	{
		x = loc.X - FMath::FRandRange(min_distanceBetweenShrubs, max_distanceBetweenShrubs);
		y = loc.Y - FMath::FRandRange(min_distanceBetweenShrubs, max_distanceBetweenShrubs);
	}

	FVector generatedLoc = FVector(x, y, lineHeight);
	FVector possibleLoc = doTrace(generatedLoc);
		
	return possibleLoc;
}

FVector AProcedural_Forest_Spawner::findLanding(FVector oldpoint)
{
	if (isValidPoint(oldpoint))
		return oldpoint;
	else
	{
		float dist = max_distanceBetweenTrees;

		if (dist <= 0)
		{
			dist *= -1;
		}

		oldpoint.X = oldpoint.X + dist;
		oldpoint.Y = currentRowIn_Y;
		oldpoint.Z = lineHeight; 

		return findLanding(oldpoint);
	}
}

//-------------------------------------------------------------------------------------------------------------------
// Spawner Hight Related Checker Service     																		-
//-------------------------------------------------------------------------------------------------------------------
bool AProcedural_Forest_Spawner::checkNextStep(FVector pointUsed)
{
	float dist = max_distanceBetweenTrees;
	pointUsed.X = pointUsed.X + dist;
	pointUsed.Y = currentRowIn_Y; 
	pointUsed.Z = lineHeight; 

	if (isValidPoint(pointUsed))
		bNextStepWillMiss = false;
	else
		bNextStepWillMiss = true; 
	
	return  bNextStepWillMiss;
}

bool AProcedural_Forest_Spawner::isValidPoint(FVector location)
{
	FVector startPoint = location;
	FVector endPoint = startPoint + (this->GetActorUpVector() * -1 * lineHeight);

	FHitResult Hit = FHitResult();

	worldTrace(Hit, startPoint, endPoint);

	bool result = validPoint(Hit, startPoint, endPoint);

	return result;
}

FVector AProcedural_Forest_Spawner::doTrace(FVector location)
{
	FVector startPoint = location;
	FVector endPoint = startPoint + (this->GetActorUpVector() * -1 * lineHeight); 

	FHitResult Hit = FHitResult();

	worldTrace(Hit, startPoint, endPoint); 

	return getImpacePoint(Hit, startPoint, endPoint);
}

bool AProcedural_Forest_Spawner::validPoint(FHitResult &hit, FVector origin, FVector traceDir)
{
	if (hit.bBlockingHit)
	{
		if (bWantDebugText)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Valid Point: ") + hit.GetActor()->GetName());
		}

		if (hit.GetActor()->GetName().Contains(FString("Landscape_0"), ESearchCase::IgnoreCase, ESearchDir::FromStart))
			return true;
		else
			return false; 
	}
	else
		return false; 
}

bool AProcedural_Forest_Spawner::worldTrace(FHitResult &hit, FVector startPoint, FVector endPoint)
{
	static FName traceTag = FName(TEXT("ForestTrace"));

	FCollisionQueryParams TraceParams(traceTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FCollisionObjectQueryParams objectQuery; 
	objectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
	objectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);


	if (bWantDebugLines)
		DrawDebugLine(this->GetWorld(), startPoint, endPoint, FColor::Red, true, timerIterationTime + additionDebugLineTime, 10.f);

	bool result = GetWorld()->LineTraceSingle(hit, startPoint, endPoint, TraceParams, objectQuery);
		
	return result;
}

FVector AProcedural_Forest_Spawner::getImpacePoint(FHitResult &hit, FVector origin, FVector traceDir)
{
	return hit.ImpactPoint; 
}
