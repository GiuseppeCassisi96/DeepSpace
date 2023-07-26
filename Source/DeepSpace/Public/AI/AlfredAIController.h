// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AlfredAIController.generated.h"

class AMainEnemy;
/// <summary>
/// This class is the controller of the agent 
/// </summary>
UCLASS()
class DEEPSPACE_API AAlfredAIController : public AAIController
{
	GENERATED_BODY()
public:
	void SetAlfredController(AActor* NewOwner);
	/**
	 * @brief This function will be invoked when the NPC Reaches the desired location
	 */
	UFUNCTION()
	void NPCReachesTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	FORCEINLINE UNavigationSystemV1* GetNavSys()
	{
		return navSys;
	}
	UPROPERTY()
	TObjectPtr<AMainEnemy> ownerController;
protected:
	/**
	 * @brief The navigation system used for the pathfinding
	 */
	UPROPERTY()
	UNavigationSystemV1* navSys;
	
};
