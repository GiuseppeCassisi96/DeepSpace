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

/**
 * @brief This struct contains the whole knowledge of the enemy 
 */
struct EnemyKnowledge
{
	TArray<TObjectPtr<ABaseMain>> CharactersSeen;
	TArray<TObjectPtr<ABaseMain>> CharactersHeard;
	TArray<FVector> bonesOfCharacter;
};
class AMainEnemy;
class UCalmBT;
class UAttackBT;
class UWarningBT;
class UAlfredFSM;
/// <summary>
/// This class is responsible for the handling of the all AI. It change the FSM status and invoke the
/// action of every status. It also handles the sensing system. The sensing system is responsible for
/// hostile characters perceiving. It uses a simple fuzzy logic to decide what have to do; this means
/// an FSM status changing.
/// </summary>
UCLASS( ClassGroup=(IA), meta=(BlueprintSpawnableComponent) )
class DEEPSPACE_API UAlfred : public UActorComponent
{
	GENERATED_BODY()

public:
	//CONSTRUCTOR
	UAlfred();

	//SENSORING FUNCTIONS
	/**
	 * @brief Is a function that I bind to a delegate. Will be executed when an actor overlap the view box.
	 * It basically adds the character seen to the seenlist 
	 */
	UFUNCTION()
	void StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
		int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults);
	/**
	 * @brief Is a function that I bind to a delegate. Will be executed when an actor exit from the view box.
	 * It basically removes the character seen from the seenlist
	 */
	UFUNCTION()
	void StopVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int otherBodyIndex);
	/**
	 * @brief Is a function that I bind to a delegate. Will be executed when an actor overlap the hear sphere.
	 * It basically adds the character heard to the heardlist
	 */
	UFUNCTION()
	void StartHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
		int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults);
	/**
	 * @brief Is a function that I bind to a delegate. Will be executed when an actor exit from the hear sphere.
	 * It basically removes the character heard from the heardlist
	 */
	UFUNCTION()
	void StopHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
		int otherBodyIndex);
	/**
	 * @brief Is a function that I bind to a delegate. Will be executed when an item hits the area inside the hear sphere 
	 * @param hitLocation The location where the obj hits the area
	 * @param itemNoisiness A value that rapresents the amount of noise of the item 
	 */
	UFUNCTION()
	void ItemHitNearEnemy(FVector hitLocation, float itemNoisiness);
	/**
	 * @brief Execute the view sensor
	 */
	void EnemyView();
	/**
	 * @brief Execute the hear sensor
	 */
	void EnemyHearing();

	//INIT AI FUNCTION
	/**
	 * @brief This function is responsible to initialize the AI
	 * @param CalmBT A reference to the behavior tree for the calm state
	 * @param AttackBT A reference to the behavior tree for the attack state
	 * @param WarningBT A reference to the behavior tree for the warning state
	 * @param NoticeSomethingBT A reference to the behavior tree for the notice something state
	 * @param AlfredFSM A reference to the FSM
	 * @param enemy reference to the enemy that owns this component 
	 * @param typeDamage The type of damage 
	 */
	void InitAI(TObjectPtr<UCalmBT> CalmBT, TObjectPtr<UAttackBT> AttackBT, TObjectPtr<
		            UWarningBT> WarningBT, TObjectPtr<UNoticeSomethingBT> NoticeSomethingBT,
	            TObjectPtr<UAlfredFSM> AlfredFSM, TObjectPtr<ACharacter> enemy, TSubclassOf<UDamageType> typeDamage);

	//EVENT FUNCTIONS
	/**
	 * @brief This function will be invoked when the NPC Reaches the desired location 
	 */
	UFUNCTION()
	void NPCReachesTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	/**
	 * @brief This function will be invoked in some cases, for example one of these cases is when
	 * the warning period finish
	 */
	UFUNCTION()
	void GoToCalmState();

	
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ResetAfterDestroy(AMainEnemy* Enemy);

	/**
	 * @brief The navigation system used for the pathfinding
	 */
	UPROPERTY()
	UNavigationSystemV1* navSys;
	/**
	 * @brief The AI controller used to send a move request to the enemy 
	 */
	UPROPERTY()
	AAIController* ControllerNPC;
	/**
	 * @brief A list that keeps track of the enemies that belong to the same side
	 */
	TArray<AMainEnemy*> SameSideEnemy;
	bool isAttacked;
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
	//Fuzzy logic vars
	FuzzyLogic SeeSet, NonHearSet;
	FTimerHandle GoToCalmTimer;
	
};
