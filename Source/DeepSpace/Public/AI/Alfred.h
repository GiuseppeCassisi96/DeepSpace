// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "FuzzyLogic.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "Alfred.generated.h"

class AMainEnemy;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class DEEPSPACE_API UAlfred : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAlfred();
	UFUNCTION()
	void StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
		int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults);
	void EnemyView();
	UFUNCTION()
	void StopVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int otherBodyIndex);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void NPCReachsTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	AMainCharacter* PlayerCharacter;
	UPROPERTY()
	AMainEnemy* owner;
	UPROPERTY()
	AAIController* ControllerNPC;
	UPROPERTY()
	TArray<AActor*> actorToIgnore;
	bool bIsPlayerInTheViewBox;
	bool bHasSeen;
	TArray<FVector> bonesOfPlayer;
	FHitResult HitResult;
	int seeCount = 0;
	FuzzyLogic SeeSet;
};
