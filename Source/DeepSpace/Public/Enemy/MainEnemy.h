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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void TakeDamageFromEnemy(AActor* DamagedActor, float Damage, const UDamageType* DamageType,  AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY()
	TObjectPtr<UCalmBT> CalmBT;
	UPROPERTY()
	TObjectPtr<UAttackBT> AttackBT;
	UPROPERTY()
	TObjectPtr<UWarningBT> WarningBT;
	UPROPERTY()
	TObjectPtr<UNoticeSomethingBT> NoticeSomethingBT;
	UPROPERTY(EditAnywhere, Category= "AI")
	TObjectPtr<UAlfred> AlfredAI;
	UPROPERTY(EditAnywhere, Category = "FSM")
	TObjectPtr<UAlfredFSM> AlfredFSM;
	UPROPERTY(EditDefaultsOnly, Category = "ViewBox")
	UBoxComponent* ViewBox;
	UPROPERTY(EditDefaultsOnly, Category = "HearingSphere")
	USphereComponent* HearingSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> typeDamage;
	UPROPERTY(EditAnywhere, Category = "EnemyHealth")
	float health = 40.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
		void ResetDeath();
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate;
};


