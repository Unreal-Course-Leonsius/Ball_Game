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

	UStaticMeshComponent* Ground;
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Tile")
	FTransform GetNextTilePoint() const;
	virtual FTransform GetNextTilePoint_Implementation() const { UE_LOG(LogTemp, Warning, TEXT("VIRTUAL GetNextTilePoint")); return ArrowComponent->GetComponentTransform(); }


protected:

	//class FObjectInitializer ObjectInitializer;

	FVector NextTilePoint;

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	class UArrowComponent* ArrowComponent;


	UPROPERTY(VisibleAnywhere, Category = "Tile")
	USceneComponent* Root;
	


};
