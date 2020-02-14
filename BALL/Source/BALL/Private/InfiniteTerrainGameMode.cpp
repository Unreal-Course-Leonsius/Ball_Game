// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "InfiniteTerrainGameMode.h"
#include "Terrain/Tile.h"
#include "Containers/Array.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileLocation.SetLocation(FVector(0, 0, 0));
	//UE_LOG(LogTemp, Warning, TEXT("GameMode Construct C++"));
}

void AInfiniteTerrainGameMode::BeginPlay()
{
	Super::BeginPlay();

	GenerateTiles();
	//UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay C++"));

}

void AInfiniteTerrainGameMode::GenerateTiles()
{
	//ATile::SetTileNumverZero();

	TileCounter = 0;

	for (int32 i = 0; i < 4; i++)
	{
		if (TileBlueprints.Num() == 0) { return; }
		ATile *Prop = GetWorld()->SpawnActor<ATile>(TileBlueprints[i], TileLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Tile Name = %s;    GetNextTilePoint = %s"), *Prop->GetName(), *Prop->GetNextTilePoint().GetLocation().ToString());
		TileLocation = Prop->GetNextTilePoint();
		LastTileIndex = i;
		++TileCounter;
	}

	//UE_LOG(LogTemp, Warning, TEXT("tilenumber = %i"), TileBlueprints.Num());

}

void AInfiniteTerrainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;
	if (Timer > 10.f)
		bCanIncreaseSpeed = true;

	if (TileCounter % 20 == 0 && bCanIncreaseSpeed)
	{
		IncreaseSpeed.Broadcast();
		bCanIncreaseSpeed = false;
		Timer = 0;
		UE_LOG(LogTemp, Warning, TEXT("InfiniteGameMode BroadCast"));
	}

}

void AInfiniteTerrainGameMode::SpawnTile()
{
	int32 index;
	if (LastTileIndex == 7)
	{
		bool bRightIndex = false;
		do
		{
			index = FMath::RandRange(0.0, (float)TileBlueprints.Num());
			if (index != 8)
				bRightIndex = true;

		} while (!bRightIndex);
	}

	if (LastTileIndex == 8)
	{
		bool bRightIndex = false;
		do
		{
			index = FMath::RandRange(0.0, (float)TileBlueprints.Num());
			if (index != 8 && index != 7 && index != 6 && index != 4)
				bRightIndex = true;
		
		} 
		while (!bRightIndex);
		
	}
	else
	{
		index = FMath::RandRange(0.0, (float)TileBlueprints.Num());
	}

	//UE_LOG(LogTemp, Warning, TEXT("======================= SpawnTile Function ======================="));
	if (TileBlueprints.Num() == 0) { return; }
	ATile* Prop = GetWorld()->SpawnActor<ATile>(TileBlueprints[index], TileLocation);
	TileLocation = Prop->GetNextTilePoint();
	LastTileIndex = index;

	++TileCounter;
	UE_LOG(LogTemp, Warning, TEXT("TileNumber = %i"), TileCounter);
	//UE_LOG(LogTemp, Warning, TEXT("GetNextTilePoint = %s"), *Prop->GetNextTilePoint().GetLocation().ToString());
	
}
