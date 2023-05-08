// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "FuzzyLogic.h"
#include "NavigationSystem.h"
#include "AlfredFSM.h"
#include "BT/AlfredBTManager.h"
#include "Items/ThrowableItem.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "Alfred.generated.h"


struct EnemyKnowledge
{
	AMainCharacter* PlayerCharacter;
	TArray<FVector> bonesOfPlayer;
};
class AMainEnemy;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEEPSPACE_API UAlfred : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION()
	void StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
		int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults);
	UFUNCTION()
	void StopVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int otherBodyIndex);
	UFUNCTION()
	void StartHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
		int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults);
	UFUNCTION()
	void StopHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
		int otherBodyIndex);
	UFUNCTION()
	void NPCReachsTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	UFUNCTION()
	void ItemHitNearEnemy(FVector hitLocation, float itemNoisiness);

	// Sets default values for this component's properties
	UAlfred();
	void EnemyView();
	void EnemyHearing();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category= "FSMComponent")
	UAlfredFSM* AlfredFSM;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BTComponent")
	UAlfredBTManager* AlfredBTManager;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	AMainEnemy* owner;
	UPROPERTY()
	AAIController* ControllerNPC;
	UPROPERTY()
	TArray<AActor*> actorToIgnore;
	UPROPERTY()
	UNavigationSystemV1* navSys;
	EnemyKnowledge EnemyData;
	bool bIsPlayerInTheViewBox, bIsPlayerInTheHearingSphere;
	bool bHasSeen;
	FHitResult HitResult;
	int seeCount = 0;
	FuzzyLogic SeeSet, NonHearSet;
	
};