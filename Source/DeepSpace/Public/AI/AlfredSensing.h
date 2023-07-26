// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
#include "BaseMain.h"
#include "Enemy/MainEnemy.h"
#include "AlfredSensing.generated.h"
struct EnemyKnowledge
{
	TArray<TObjectPtr<ABaseMain>> CharactersSeen;
	TArray<TObjectPtr<ABaseMain>> CharactersHeard;
	TArray<FVector> bonesOfCharacter;
};

/// <summary>
/// This class is an Alfred AI component that is responsible to handle the all sensing system.
/// Starts and stops the hear and view sensors and executes them. This class has the so called
///	sensory functions
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEEPSPACE_API UAlfredSensing : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAlfredSensing();
	/*SENSORY FUNCTIONS : This type of function are responsible of all sensing task.
	*When a hostile entity will be added to the lists means that she was perceived.
	*When a hostile entity will be removed from the lists means that she was forgot
	*/

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
	void SetOwner(TObjectPtr<AMainEnemy> ownerComponent);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	TObjectPtr<AMainEnemy> owner;
	/**
	 * @brief A list that keeps track of the enemies that belong to the same side
	 */
	TArray<AMainEnemy*> SameSideEntity;
	EnemyKnowledge EnemyData;
	//Fuzzy logic vars
	FuzzyLogic SeeSet, NonHearSet;
	FHitResult HitResult;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	TArray<AActor*> actorToIgnore;
	int seeCount = 0;
};
