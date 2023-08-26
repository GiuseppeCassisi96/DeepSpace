// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredAIController.h"
#include "AI/AlfredSensing.h"


void AAlfredAIController::SetAlfredController(AActor* NewOwner)
{
	ownerController = Cast<AMainEnemy>(NewOwner);
	navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
}

void AAlfredAIController::NPCReachesTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	EEnemyState state = ownerController->AlfredAI->AlfredFSM->GetCurrentState();
	/*The first condition ('state == EEnemyState::Attack || state == EEnemyState::NoticeSomething')
	 *is to check if the agent has seen an hostile entity or notice some noise. The second condition
	 *is to check if the path following result is a success. The third condition is to check if the
	 *agent see the hostile entity or not.
	 */
	if ((state == EEnemyState::Attack || state == EEnemyState::NoticeSomething)
		&& Result == EPathFollowingResult::Success && !ownerController->AlfredAI->AlfredSensing->SeeSet.Defuzzification(0.65f))
	{

		//Pass to warning state: He has noticed something and now he getting worried
		ownerController->AlfredAI->AlfredFSM->GoToNewState(EEnemyState::Warning);
		ownerController->GetWorldTimerManager().SetTimer(ownerController->AlfredAI->GoToCalmTimer, ownerController->AlfredAI->AlfredFSM,
			&UAlfredFSM::GoToDefaultState, 60.0f);
	}
}


