// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "FuzzyLogic.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BT/NoticeSomethingBT.h"
#include "Components/ActorComponent.h"
#include "Alfred.generated.h"


struct EnemyKnowledge
{
	TArray<TObjectPtr<ABaseMain>> CharactersSeen;
	TArray<TObjectPtr<ABaseMain>> CharactersHeard;
	TArray<FVector> bonesOfPlayer;
};
class AMainEnemy;
class UCalmBT;
class UAttackBT;
class UWarningBT;
class UAlfredFSM;
UCLASS( ClassGroup=(IA), meta=(BlueprintSpawnableComponent) )
class DEEPSPACE_API UAlfred : public UActorComponent
{
	GENERATED_BODY()

public:
	//CONSTRUCTOR
	UAlfred();

	//SENSORING FUNCTIONS
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
	void EnemyView();
	void EnemyHearing();

	//EVENT FUNCTIONS
	UFUNCTION()
	void NPCReachsTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	UFUNCTION()
	void ItemHitNearEnemy(FVector hitLocation, float itemNoisiness);
	UFUNCTION()
	void GoToCalmState();

	//INIT AI FUNCTION
	void InitAI(TObjectPtr<UCalmBT> CalmBT, TObjectPtr<UAttackBT> AttackBT,
	            TObjectPtr<UAlfredFSM> AlfredFSM, TObjectPtr<ACharacter> enemy, TSubclassOf<UDamageType> typeDamage, TObjectPtr<
		            UWarningBT> WarningBT, TObjectPtr<UNoticeSomethingBT> NoticeSomethingBT);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetIsAttacked(TObjectPtr<AMainEnemy> EnemyThatAttack);
	void ResetAfterDestroy(AMainEnemy* Enemy);

	UPROPERTY()
	UNavigationSystemV1* navSys;
	UPROPERTY()
	AAIController* ControllerNPC;
	UPROPERTY(EditAnywhere, Category="AIProperties")
	TArray<AMainEnemy*> SameSideEnemy;
	bool isAttacked;
	bool bHasSeen;
	bool bHasNoticeSomething;
	EnemyKnowledge EnemyData;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	AMainEnemy* owner;
	UPROPERTY()
	TArray<AActor*> actorToIgnore;
	FHitResult HitResult;
	int seeCount = 0;
	FuzzyLogic SeeSet, NonHearSet;
	FTimerHandle GoToCalmTimer;
	
};
