// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "FuzzyLogic.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BT/NoticeSomethingBT.h"
#include "Components/ActorComponent.h"
#include "BT/AlfredBT.h"
#include "Alfred.generated.h"

/**
 * @brief This struct contains the whole knowledge of the enemy 
 */
//struct EnemyKnowledge
//{
//	TArray<TObjectPtr<ABaseMain>> CharactersSeen;
//	TArray<TObjectPtr<ABaseMain>> CharactersHeard;
//	TArray<FVector> bonesOfCharacter;
//};
class AMainEnemy;
class UCalmBT;
class UAttackBT;
class UWarningBT;
class UAlfredFSM;
class UAlfredBT;
class UAlfredSensing;
/// <summary>
/// This class is the central point of my AI. It executes sensors execution functions and initialize
///	the whole AI. It also creates the other AI component 
/// </summary>
UCLASS( ClassGroup=(IA), meta=(BlueprintSpawnableComponent) )
class DEEPSPACE_API UAlfred : public UActorComponent
{
	GENERATED_BODY()

public:
	//CONSTRUCTOR
	UAlfred();

	//INIT AI FUNCTION
	/**
	 * @brief This function is responsible to initialize the AI
	 * @param CalmBT A reference to the behavior tree for the calm state
	 * @param AttackBT A reference to the behavior tree for the attack state
	 * @param WarningBT A reference to the behavior tree for the warning state
	 * @param NoticeSomethingBT A reference to the behavior tree for the notice something state
	 */
	void InitAI(TObjectPtr<UCalmBT> CalmBT, TObjectPtr<UAttackBT> AttackBT, TObjectPtr<
		            UWarningBT> WarningBT, TObjectPtr<UNoticeSomethingBT> NoticeSomethingBT);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void GoToCalmStateAfterDestroy(AMainEnemy* Enemy);

	UPROPERTY()
	UAlfredBT* AlfredBT;
	UPROPERTY(EditAnywhere, Category = "AI|FSM")
	UAlfredFSM* AlfredFSM;
	UPROPERTY()
	TObjectPtr<UAlfredSensing>  AlfredSensing;
	bool isAttacked;
	FTimerHandle GoToCalmTimer;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	AMainEnemy* owner;
	
};
