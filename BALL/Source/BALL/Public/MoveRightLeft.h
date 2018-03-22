// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MoveRightLeft.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWallMoving);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALL_API UMoveRightLeft : public UActorComponent
{
	GENERATED_BODY()

private:

	// Owner Actor pointer
	AActor * Owner = nullptr;

	FVector OwnerOriginLocation;

	float TickTime = 0.f;

	bool bLeft = false;

public:	
	// Sets default values for this component's properties
	UMoveRightLeft();

	// Dynamic Multicast Delegate in Blueprint
	UPROPERTY(BlueprintAssignable)
	FWallMoving Right;

	// Dynamic Multicast Delegate in Blueprint
	UPROPERTY(BlueprintAssignable)
	FWallMoving Left;

	UPROPERTY(BlueprintReadWrite, Category = "Ready To Move")
	bool bRight = true;

	UFUNCTION(BlueprintCallable, Category = "Ready To Move")
	void ReadyMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void MoveLeftRight();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
