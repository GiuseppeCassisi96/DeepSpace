// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/AttackBT.h"

#include "Kismet/GameplayStatics.h"



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
	//INSTANCIES CREATION PHASE:
		//TASKS
	checkDistanceMT = NewObject<UTaskBT>();
	followThePlayer = NewObject<UTaskBT>();
	attackThePlayer = NewObject<UTaskBT>();
	wait = NewObject<UTaskBT>();

		//COMPOSITE TASK
	sequenceSelector = NewObject<USelectorBT>();
	firstSequence = NewObject<USequenceBT>();
	secondSequence = NewObject<USequenceBT>();

	//BINDING PHASE:
	checkDistanceMT->Task.BindUFunction(this, FName("CheckDistanceMoreThan"));
	followThePlayer->Task.BindUFunction(this, FName("GoTowardsThePlayer"));
	attackThePlayer->Task.BindUFunction(this, TEXT("Attack"));
	wait->Task.BindUFunction(this, TEXT("WaitFunc"));

	//ADDING PHASE:
	firstSequence->Tasks.Add(checkDistanceMT);
	firstSequence->Tasks.Add(followThePlayer);
	secondSequence->Tasks.Add(attackThePlayer);
	secondSequence->Tasks.Add(wait);
	sequenceSelector->Tasks.Add(firstSequence);
	sequenceSelector->Tasks.Add(secondSequence);

	//PARAM SETTING PHASE:
	RootTask = sequenceSelector;
	bCanAttack = true;
	playerRefBT = playerRef;
	typeDamage = type;
}

//Condition
ETaskExeState UAttackBT::CheckDistanceMoreThan()
{
	float currentDinstance = FVector::Dist(playerRefBT->GetActorLocation(), ownerBT->GetActorLocation());
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red,
		FString::Printf(TEXT("Current Dist: %f"), currentDinstance));
	if (currentDinstance > 82.0f)
	{
		TreeExeState = ETaskExeState::Success;
		return ETaskExeState::Success;
	}
	TreeExeState = ETaskExeState::Fail;
	return ETaskExeState::Fail;
}

//Action
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

//Action
ETaskExeState UAttackBT::Attack()
{
	if(bCanAttack)
		UGameplayStatics::ApplyDamage(playerRefBT, 10.0f,
			ownerBT->GetController(), ownerBT, typeDamage);
	return ETaskExeState::Success;
}

//Action
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
