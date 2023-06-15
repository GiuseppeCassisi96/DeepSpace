// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/AttackBT.h"

#include "Kismet/GameplayStatics.h"
#include "BaseMain.h"
#include "Materials/MaterialInstanceDynamic.h"


ETaskExeState UAttackBT::RunTree()
{
	
	if (!bIsStopped)
	{
		MaterialInstance->SetVectorParameterValue(TEXT("ColorLight"), FLinearColor::Red);
		ownerBT->GetMesh()->SetMaterial(5, MaterialInstance);
		ownerBT->GetMesh()->SetMaterial(8, MaterialInstance);
		TreeExeState = RootTask->RunTask();
		return TreeExeState;
	}
	TreeExeState = ETaskExeState::Stopped;
	return ETaskExeState::Stopped;
}

void UAttackBT::StopTree()
{

	Super::StopTree();
}

void UAttackBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<ACharacter> playerRef, TSubclassOf<UDamageType> type)
{
	Super::InitTree(owner, navSys);
	//INSTANCIES CREATION PHASE: Here I create the node instances
		//TASKS
	checkLifeOfTarget = NewObject<UTaskBT>();
	checkDistanceMT = NewObject<UTaskBT>();
	followThePlayer = NewObject<UTaskBT>();
	attackThePlayer = NewObject<UTaskBT>();
	wait = NewObject<UTaskBT>();

		//COMPOSITE TASK
	sequenceSelector = NewObject<USelectorBT>();
	firstSequence = NewObject<USequenceBT>();
	secondSequence = NewObject<USequenceBT>();

	//BINDING PHASE: Here I bind the task with the function
	checkLifeOfTarget->Task.BindUFunction(this, FName("CheckLife"));
	checkDistanceMT->Task.BindUFunction(this, FName("CheckDistanceMoreThan"));
	followThePlayer->Task.BindUFunction(this, FName("GoTowardsThePlayer"));
	attackThePlayer->Task.BindUFunction(this, TEXT("Attack"));
	wait->Task.BindUFunction(this, TEXT("WaitFunc"));

	//ADDING PHASE: Here I add the task to the sequence array and sequence to the selector array
	firstSequence->Tasks.Add(checkLifeOfTarget);
	firstSequence->Tasks.Add(checkDistanceMT);
	firstSequence->Tasks.Add(followThePlayer);
	secondSequence->Tasks.Add(checkLifeOfTarget);
	secondSequence->Tasks.Add(attackThePlayer);
	secondSequence->Tasks.Add(wait);
	sequenceSelector->Tasks.Add(firstSequence);
	sequenceSelector->Tasks.Add(secondSequence);

	//PARAM SETTING PHASE:
	RootTask = sequenceSelector;
	bCanAttack = true;
	playerRefBT = playerRef;
	typeDamage = type;
	MaterialInstance = UMaterialInstanceDynamic::Create(ownerBT->GetMesh()->GetMaterial(5),
		NULL);
}

//Condition
ETaskExeState UAttackBT::CheckLife()
{
	if (Cast<ABaseMain>(playerRefBT)->health <= 0.0f)
		return ETaskExeState::Fail;
	return ETaskExeState::Success;
}

//Condition
ETaskExeState UAttackBT::CheckDistanceMoreThan()
{
	float currentDinstance = FVector::Dist(playerRefBT->GetActorLocation(), ownerBT->GetActorLocation());
	if (currentDinstance > 92.0f)
	{
		return ETaskExeState::Success;
	}
	return ETaskExeState::Fail;
}

//Action
ETaskExeState UAttackBT::GoTowardsThePlayer()
{
	AIController->MoveToLocation(playerRefBT->GetActorLocation());
	return ETaskExeState::Success;//Success !
	
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
		float waitTime = FMath::RandRange(2.0f, 5.0f);
		ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this, &UAttackBT::SetCanAttack, waitTime, false);
	}
	return ETaskExeState::Success;
}

void UAttackBT::SetCanAttack()
{
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
	bCanAttack = true;
}
