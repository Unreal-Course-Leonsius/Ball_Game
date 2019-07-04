// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

#define PURE_VIRTUAL(func,extra) { LowLevelFatalError(TEXT("Pure virtual not implemented (%s)"), TEXT(#func)); extra }

UCLASS()
class BALL_API ATile : public AActor
{
	GENERATED_BODY()

private:

	//UStaticMeshComponent* Ground;
	FVector ArrowLocation = FVector(5000.f, 0.f, 0.f);

	int32 maxSpawnNumber = 0; // for Spawn Actor
	int32 indexLocation = 0; // for Spawn Actor's LocatonArray

private:

	int32 SetUniqueIndext(int32 MaxRange, TArray<bool> &IsTheLocationFree);

	void FillBoolArray(TArray<bool> &IsTheLocationFree, int32 length);

	void GenerateSpawnActor_RandomLocation(TSubclassOf<AActor>ToSpawn, FVector SpawnLocation);
	
public:	

	// Sets default values for this actor's properties
	ATile(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Tile")
	FTransform GetNextTilePoint() const;
	virtual FTransform GetNextTilePoint_Implementation() const { UE_LOG(LogTemp, Warning, TEXT("VIRTUAL GetNextTilePoint")); return ArrowComponent->GetComponentTransform(); }

protected:

	UPROPERTY(VisibleAnywhere, Category = "SpawnActor")  // VisibleAnywhere, Category = "SpawnActor"
	TArray<AActor*> GarbageObject;

	FVector NextTilePoint;

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	class UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	USceneComponent* Root;

	FTimerDelegate TimerDel;
	FTimerHandle Timer;

	float DelationTime = 0.1f;

protected:

	//class FObjectInitializer ObjectInitializer;


	UFUNCTION(BlueprintCallable, Category = "Spwan Actor")
	void SpawnActorsRandomLocation(TSubclassOf<AActor>ToSpawn, int32 MinSpawn, int32 MaxSpawn, TArray<FVector> SpawnLocations);

	UFUNCTION(BlueprintCallable, Category = "Spwan Actor")
	void SpawnActorConcretLocation(TSubclassOf<AActor>ToSpawn, int32 MaxSpawn, TArray<FVector> SpawnLocations);


	UFUNCTION(BlueprintCallable, Category = "Spwan Actor")
	void DestroySpawnActors();  // Parametere == TArray<AActor*> Garbage

	UFUNCTION()
	void GenerateSpawnActor_ConcretLocation(TSubclassOf<AActor>ToSpawn, TArray<FVector> SpawnLocation);

	UFUNCTION(BlueprintCallable, Category = "Spwan Actor")
	void SpawnActors_TimeLineObjects(TSubclassOf<AActor>ToSpawn, TArray<AActor*> &SpawnActors, TArray<FVector> SpawnLocations);

};