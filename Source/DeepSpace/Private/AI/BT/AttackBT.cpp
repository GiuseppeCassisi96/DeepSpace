// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/AttackBT.h"

#include "Kismet/GameplayStatics.h"


UAttackBT::UAttackBT()
{
	//TASKS
	checkDistanceMT = NewObject<UTaskBT>();
	followThePlayer = NewObject<UTaskBT>();
	attackThePlayer = NewObject<UTaskBT>();
	wait = NewObject<UTaskBT>();

	//COMPOSITE TASK
	sequenceSelector = NewObject<USelectorBT>();
	firstSequence = NewObject<USequenceBT>();
	secondSequence = NewObject<USequenceBT>();

	firstSequence->Tasks.Add(checkDistanceMT);
	firstSequence->Tasks.Add(followThePlayer);
	secondSequence->Tasks.Add(attackThePlayer);
	secondSequence->Tasks.Add(wait);

	sequenceSelector->Tasks.Add(firstSequence);
	sequenceSelector->Tasks.Add(secondSequence);

	RootTask = sequenceSelector;
	bCanAttack = true;
}

ETaskExeState UAttackBT::RunTree()
{
	if(!bIsStopped)
		return RootTask->RunTask();
	return ETaskExeState::Fail;
}

void UAttackBT::StopTree()
{
	Super::StopTree();
}

void UAttackBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<ACharacter> playerRef, TSubclassOf<UDamageType> type)
{
	Super::InitTree(owner, navSys);
	checkDistanceMT->Task.BindUFunction(this, FName("CheckDistanceMoreThan"));
	followThePlayer->Task.BindUFunction(this, FName("GoTowardsThePlayer"));
	attackThePlayer->Task.BindUFunction(this, TEXT("Attack"));
	wait->Task.BindUFunction(this, TEXT("WaitFunc"));
	playerRefBT = playerRef;
	typeDamage = type;
}

ETaskExeState UAttackBT::CheckDistanceMoreThan()
{
	float currentDinstance = FVector::Dist(playerRefBT->GetActorLocation(), ownerBT->GetActorLocation());
	if (currentDinstance > 72.0f)
	{
		TreeExeState = ETaskExeState::Success;
		return ETaskExeState::Success;
	}
	TreeExeState = ETaskExeState::Fail;
	return ETaskExeState::Fail;
}

ETaskExeState UAttackBT::GoTowardsThePlayer()
{
	if(AIController->MoveToLocation(playerRefBT->GetActorLocation(),
		2.0f) == EPathFollowingRequestResult::RequestSuccessful)
	{
		TreeExeState = ETaskExeState::Success;
		return ETaskExeState::Success;//Success !
	}
	TreeExeState = ETaskExeState::Fail;
	return ETaskExeState::Fail;
}

ETaskExeState UAttackBT::Attack()
{
	if(bCanAttack)
		UGameplayStatics::ApplyDamage(playerRefBT, 10.0f,
			ownerBT->GetController(), ownerBT, typeDamage);
	return ETaskExeState::Success;
}

ETaskExeState UAttackBT::WaitFunc()
{
	if(bCanAttack)
	{
		bCanAttack = false;
		ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this, &UAttackBT::SetCanAttack, 3.0f, false);
	}
	return ETaskExeState::Success;
}

void UAttackBT::SetCanAttack()
{
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
	bCanAttack = true;
}
