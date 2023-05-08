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

	//TASKS BINDING
	checkDistanceMT->BindTask(this, "CheckDistanceMoreThan");
	followThePlayer->BindTask(this, "GoTowardsThePlayer");
	attackThePlayer->BindTask(this, "Attack");
	wait->BindTask(this, "WaitFunc");

	firstSequence->Tasks.Add(checkDistanceMT);
	firstSequence->Tasks.Add(followThePlayer);
	secondSequence->Tasks.Add(attackThePlayer);
	secondSequence->Tasks.Add(wait);

	sequenceSelector->Tasks.Add(firstSequence);
	sequenceSelector->Tasks.Add(secondSequence);

	RootTask = sequenceSelector;
	bCanAttack = true;
}

int UAttackBT::RunTree()
{
	if(!bIsStopped)
		return RootTask->RunTask();
	return -1;
}

void UAttackBT::StopTree()
{
	Super::StopTree();
}

void UAttackBT::InitTree(ACharacter* owner, UNavigationSystemV1* navSys, ACharacter* playerRef, TSubclassOf<UDamageType> type)
{
	Super::InitTree(owner, navSys);
	playerRefBT = playerRef;
	typeDamage = type;
}

int UAttackBT::CheckDistanceMoreThan()
{
	float currentDinstance = FVector::Dist(playerRefBT->GetActorLocation(), ownerBT->GetActorLocation());
	if (currentDinstance > 72.0f)
	{
		TreeExeCode = 0;
		return 0;
	}
	TreeExeCode = -1;
	return -1;
}

int UAttackBT::GoTowardsThePlayer()
{
	if(AIController->MoveToLocation(playerRefBT->GetActorLocation(),
		2.0f) == EPathFollowingRequestResult::RequestSuccessful)
	{
		TreeExeCode = 0;
		return 0;//Success !
	}
	TreeExeCode = -1;
	return -1;
}

int UAttackBT::Attack()
{
	if(bCanAttack)
		UGameplayStatics::ApplyDamage(playerRefBT, 10.0f,
			ownerBT->GetController(), ownerBT, typeDamage);
	return 0;
}

int UAttackBT::WaitFunc()
{
	if(bCanAttack)
	{
		bCanAttack = false;
		ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this, &UAttackBT::SetCanAttack, 3.0f, false);
	}
	return 0;
}

void UAttackBT::SetCanAttack()
{
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
	bCanAttack = true;
}
