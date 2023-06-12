// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "BaseMain.generated.h"

enum class AnimState
{
	Idle = 0,
	Walk = 300,
	Run = 500,
	Crouch = 0,
};

UENUM()
enum class CharacterType
{
	Enemy,
	Player,
	Ally
};
UCLASS()
class DEEPSPACE_API ABaseMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMain();
	virtual TArray<FVector> GetCharacterBones();
	virtual void SetupBones();
	UPROPERTY(EditDefaultsOnly, Category="Bones")
	TArray<FName> bonesNames;
	UPROPERTY(EditAnywhere, Category = "CharacterType")
	CharacterType Chartype;
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
