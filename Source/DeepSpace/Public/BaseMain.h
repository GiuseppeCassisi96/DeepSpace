// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "BaseMain.generated.h"

/**
 * @brief Are the animation states. The integer value associated represents the
 * amount of noisnes of each anim state
 */
enum class AnimState
{
	Idle = 0,
	Walk = 300,
	Run = 500,
	EnemyDefaultState = 400,
	Crouch = -1500,
};
/// <summary>
/// Are the all possible char type
///	Player: is the main player
///	Ally: is an ally of the player and will attack the other enemies
///	Enemy: is the classic enemy. Will attack the player and the allies of the player
/// </summary>
UENUM()
enum class ECharacterType
{
	Enemy,
	Player,
	Ally
};
UCLASS()
/// <summary>
/// This is the base class for all character present in the prototype 
/// </summary>
class DEEPSPACE_API ABaseMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMain();
	/**
	 * @brief Returns the updated position of character's bones
	 * @return An array of positions
	 */
	virtual TArray<FVector> GetCharacterBones();
	/**
	 * @brief Gets the bones locations based on their names 
	 */
	virtual void SetupBones();

	UPROPERTY(EditDefaultsOnly, Category="Bones")
	TArray<FName> bonesNames;
	UPROPERTY(EditAnywhere, Category = "CharacterType")
	ECharacterType Chartype;
	UPROPERTY(EditAnywhere, Category = "Health")
	float health = 40.0f;

	TArray<FVector> bones;
	AnimState state;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
