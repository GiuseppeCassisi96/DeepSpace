// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "BaseMain.h"
#include "Engine/Engine.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Components/InputComponent.h"
#include "Items/ThrowableItem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"



UCLASS()
class DEEPSPACE_API AMainCharacter : public ABaseMain
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& actionValue);
	void Rotation(const FInputActionValue& actionValue);
	void Crouch(const FInputActionValue& actionValue);
	void Aim(const FInputActionValue& actionValue);
	void Run(const FInputActionValue& actionValue);
	void Throw(const FInputActionValue& actionValue);
	virtual TArray<FVector> GetCharacterBones() override;
	void SetState(AnimState newState);
	UFUNCTION()
	void TakeDamageFromEnemy(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);
	

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RotationAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ThrowAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	float movementSpeed = 0.3f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float rotationSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* aimMontage;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	UPROPERTY()
	UAnimInstance* AnimInstance;
	UPROPERTY(EditDefaultsOnly, Category= "Item")
	TSubclassOf<AActor> ThrowableObj;
	float forwardVelocity;
	bool isCrouch, isMovementLeft, isMovementRight, isMovementBack, isCrouchMovementLeft,
	isCrouchMovementRight, isCrouchMovementBack, isAiming, isRun;
	float myDeltaTime;
	FTransform spineTrasform;
	FRotator spineRotation;
	float springArmLenght;
	float startMovementSpeed;
	float forwardMovementValue, rightMovementValue;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
