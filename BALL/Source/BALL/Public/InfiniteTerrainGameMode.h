// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BALLGameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeSpeed);

/**
 * 
 */
UCLASS()
class BALL_API AInfiniteTerrainGameMode : public ABALLGameModeBase
{
	GENERATED_BODY()

public:

	AInfiniteTerrainGameMode();

public:

	UPROPERTY(BlueprintAssignable)
	FChangeSpeed IncreaseSpeed;
	
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Tile Array")
	TArray<TSubclassOf<class ATile>> TileBlueprints;
	
	FTransform TileLocation;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void GenerateTiles();


	UFUNCTION(BlueprintCallable, Category= "Tile")
	void SpawnTile();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tile")
	void EventFromBeginPlay();

private:

	int32 LastTileIndex;

	bool bCanIncreaseSpeed = false;
	float Timer = 0;
	int32 TileCounter = 0;

};
