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
	UE_LOG(LogTemp, Warning, TEXT("GameMode Construct C++"));
}

void AInfiniteTerrainGameMode::BeginPlay()
{
	Super::BeginPlay();

	GenerateTiles();
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay C++"));

}

void AInfiniteTerrainGameMode::GenerateTiles()
{

	UE_LOG(LogTemp, Warning, TEXT("GenerateTiles Function ======================="));
	for (int32 i = 0; i < 4; i++)
	{
		if (TileBlueprints.Num() == 0) { return; }
		ATile *Prop = GetWorld()->SpawnActor<ATile>(TileBlueprints[i], TileLocation);
		UE_LOG(LogTemp, Warning, TEXT("Tile Name = %s;    GetNextTilePoint = %s"), *Prop->GetName(), *Prop->GetNextTilePoint().GetLocation().ToString());
		TileLocation = Prop->GetNextTilePoint();
	}

	UE_LOG(LogTemp, Warning, TEXT("tilenumber = %i"), TileBlueprints.Num());

}

void AInfiniteTerrainGameMode::SpawnTile()
{
	int32 index = FMath::RandRange(0.0, (float)TileBlueprints.Num());
	UE_LOG(LogTemp, Warning, TEXT("======================= SpawnTile Function ======================="));
	if (TileBlueprints.Num() == 0) { return; }
	ATile* Prop = GetWorld()->SpawnActor<ATile>(TileBlueprints[index], TileLocation);
	TileLocation = Prop->GetNextTilePoint();
	UE_LOG(LogTemp, Warning, TEXT("GetNextTilePoint = %s"), *Prop->GetNextTilePoint().GetLocation().ToString());
	
}
