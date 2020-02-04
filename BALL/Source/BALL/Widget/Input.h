// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Input.generated.h"

/**
 * 
 */
UCLASS()
class BALL_API UInput : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(meta = (BindWidget))
	class UButton* Left;

	UPROPERTY(meta = (BindWidget))
	class UButton* Right;

protected:

	virtual bool Initialize() override;

protected:

	UFUNCTION()
	void Left_Direction();

	UFUNCTION()
	void Right_Direction();

	UFUNCTION()
	void Stop();

private:

	class ABall_C * Player;
	
};
