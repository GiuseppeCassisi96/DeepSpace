// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/MainAnimInstance.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::UpdateAnimProperties()
{
	if(MainCharacter)
	{
		velocity = MainCharacter->forwardVelocity;
		isCrouch = MainCharacter->isCrouch;
		isMovementLeft = MainCharacter->isMovementLeft;
		isMovementRight = MainCharacter->isMovementRight;
		isMovementBack = MainCharacter->isMovementBack;
		isCrouchMovementLeft = MainCharacter->isCrouchMovementLeft;
		isCrouchMovementRight = MainCharacter->isCrouchMovementRight;
		isCrouchMovementBack = MainCharacter->isCrouchMovementBack;
	}
}
