// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "AI/Alfred.h"
#include "AI/AlfredFSM.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "MainEnemy.generated.h"


UCLASS()
class DEEPSPACE_API AMainEnemy : public ABaseMain
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual TArray<FVector> GetCharacterBones() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void TakeDamageFromEnemy(AActor* DamagedActor, float Damage, const UDamageType* DamageType,  AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(EditAnywhere, Category= "AI")
	TObjectPtr<UAlfred> AlfredAI;

	UPROPERTY(EditDefaultsOnly, Category = "ViewBox")
	UBoxComponent* ViewBox;
	UPROPERTY(EditDefaultsOnly, Category = "HearingSphere")
	USphereComponent* HearingSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> typeDamage;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	/**
	 * @brief The AI controller used to send a move request to the enemy
	 */
	UPROPERTY()
	AAlfredAIController* ControllerNPC;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void ResetDeath(AMainEnemy* Enemy);
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate;
	
};




