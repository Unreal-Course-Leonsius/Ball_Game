// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BALLGameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BALL_API AInfiniteTerrainGameMode : public ABALLGameModeBase
{
	GENERATED_BODY()

public:

	AInfiniteTerrainGameMode();
	
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Tile Array")
	TArray<TSubclassOf<class ATile>> TileBlueprints;
	
	FTransform TileLocation;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateTiles();


	UFUNCTION(BlueprintCallable, Category= "Tile")
	void SpawnTile();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tile")
	void EventFromBeginPlay();

private:

	int32 LastTileIndex;

};
