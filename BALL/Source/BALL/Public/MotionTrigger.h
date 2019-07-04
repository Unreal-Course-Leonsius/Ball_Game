// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MotionTrigger.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWallMoving);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWallMovingTrigger, int32, WallIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALL_API UMotionTrigger : public UActorComponent
{
	GENERATED_BODY()

private:

	// Owner Actor pointer
	AActor * Owner = nullptr;

	FVector OwnerOriginLocation;

	float TickTime = 0.f;

public:	
	// Sets default values for this component's properties
	UMotionTrigger();

	// Dynamic Multicast Delegate in Blueprint
	UPROPERTY(BlueprintAssignable)
	FWallMoving Move;  // we need it only one time because TimeLine is looping

	/// we need it for Growing Tile
	UPROPERTY(BlueprintAssignable)
	FWallMoving MovingRightWall_1;

	UPROPERTY(BlueprintAssignable)
	FWallMoving MovingRightWall_2;

	UPROPERTY(BlueprintAssignable)
	FWallMoving MovingLeftWall;

	UPROPERTY()
	TArray<FWallMoving> WallMovingTrigger;

	

	bool bRight = true;  // UPROPERTY(BlueprintReadWrite, Category = "Ready To Move")

protected:

	FTimerDelegate TimerDel;
	FTimerHandle Timer;

	int32 growing_Wall_Number = 3;
	int32 growing_Wall_Number_Index = 0;
	float growing_Delation_Time = 0.2f;


protected:

	UFUNCTION(BlueprintCallable, Category = "Growing Wall")
	void GrowingWall();

	UFUNCTION()
	void StartGrowing();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//void MoveLeftRight();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
