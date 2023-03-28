// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacter.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable)
	void UpdateAnimProperties();
	UPROPERTY()
	AMainCharacter* MainCharacter;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	float velocity;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	bool isCrouch;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	bool isMovementLeft;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	bool isMovementRight;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	bool isMovementBack;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	bool isCrouchMovementLeft;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	bool isCrouchMovementRight;
	UPROPERTY(BlueprintReadOnly, Category = "AnimProperties")
	bool isCrouchMovementBack;
};
