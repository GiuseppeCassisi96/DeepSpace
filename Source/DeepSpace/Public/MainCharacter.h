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
	/**
	 * @brief Implements the move action of the main character
	 * @param actionValue Is a value based on the action's settings and user input
	 */
	void Move(const FInputActionValue& actionValue);
	/**
	 * @brief Implements the rotation action of the main character
	 * @param actionValue Is a value based on the action's settings and user input
	 */
	void Rotation(const FInputActionValue& actionValue);
	/**
	 * @brief Implements the crouch action of the main character
	 * @param actionValue Is a value based on the action's settings and user input
	 */
	void Crouch(const FInputActionValue& actionValue);
	/**
	 * @brief Implements the aim action of the main character
	 * @param actionValue Is a value based on the action's settings and user input
	 */
	void Aim(const FInputActionValue& actionValue);
	/**
	 * @brief Implements the run action of the main character
	 * @param actionValue Is a value based on the action's settings and user input
	 */
	void Run(const FInputActionValue& actionValue);
	/**
	 * @brief Implements the throw action of the main character
	 * @param actionValue Is a value based on the action's settings and user input
	 */
	void Throw(const FInputActionValue& actionValue);
	/**
	 * @brief Sets the new animation state
	 * @param newState New animation state
	 */
	void SetState(AnimState newState);
	/**
	 * @brief Returns the updated position of character's bones
	 * @return An array of positions
	 */
	virtual TArray<FVector> GetCharacterBones() override;
	/**
	 * @brief Is a method that I bind to a delegate. Will be triggered every time the main character
	 *        receives a damage
	 * @param DamagedActor The actor that receives the damage
	 * @param Damage The amount of damage
	 * @param DamageType The type of damage
	 * @param InstigatedBy The controller of the enemy that causes the damage
	 * @param DamageCauser The enemy that causes the damage
	 */
	UFUNCTION()
	void TakeDamageFromEnemy(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);
	
	//Camera component of main char 
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;
	//Spring Arm component of main char 
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY()
	APlayerController* PlayerController;
	//INPUT VARS
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
	//The aim animation montage
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* aimMontage;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	UPROPERTY()
	UAnimInstance* AnimInstance;
	//The reference to the obj that the main char can throws
	UPROPERTY(EditDefaultsOnly, Category= "Item")
	TSubclassOf<AActor> ThrowableObj;
	float forwardVelocity;
	/*These are boolean vars for animation state machine.They are used in the animation
	 *state machine of the animBP*/
	bool isCrouch, isMovementLeft, isMovementRight, isMovementBack, isCrouchMovementLeft,
	isCrouchMovementRight, isCrouchMovementBack, isAiming, isRun;

	float myDeltaTime;
	FTransform spineTrasform;
	//Is a the rotation of spine. It is used inside the blueprint of AnimBP
	FRotator spineRotation;
	float springArmLenght;
	float startMovementSpeed;
	float forwardMovementValue, rightMovementValue;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
