// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/CalmBT.h"

#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


ETaskExeState UCalmBT::RunTree()
{
	if (!bIsStopped)
	{
		MaterialInstance->SetVectorParameterValue(TEXT("ColorLight"), FLinearColor::Green);
		ownerBT->GetMesh()->SetMaterial(5, MaterialInstance);
		ownerBT->GetMesh()->SetMaterial(8, MaterialInstance);
		TreeExeState = RootTask->RunTask();
		if(TreeExeState == ETaskExeState::TryAgain)
		{
			//I create a timer to implement until fail decorator
			ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this,
				FTimerDelegate::TMethodPtr<UCalmBT>(&UCalmBT::RunTree),
				10.0f, false);
		}
		return TreeExeState;
	}
	TreeExeState = ETaskExeState::Stopped;
	return ETaskExeState::Stopped;
}




void UCalmBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys)
{
	Super::InitTree(owner, navSys);
	//INSTANCIES CREATION PHASE: Here I create the node instances
		//TASKS
	TaskIsReachable = NewObject<UTaskBT>(this, FName("FTCalmBT"));
	TaskGoToRandPosition = NewObject<UTaskBT>(this, FName("STCalmBT"));
		//COMPOSITE TASKS
	FirstSequence = NewObject<USequenceBT>(this, FName("FSCalmBT"));
		//DECORATOR
	CalmUntilFail = NewObject<UUntilFailBT>(this, FName("UFCalmBT"));

	//BINDING PHASE: Here I bind the task with the function
	TaskIsReachable->Task.BindUFunction(this, TEXT("IsReachable"));
	TaskGoToRandPosition->Task.BindUFunction(this, TEXT("GoToRandPosition"));

	//ADDING PHASE: Here I add the task to the sequence array
	FirstSequence->Tasks.Add(TaskIsReachable);
	FirstSequence->Tasks.Add(TaskGoToRandPosition);

	//PARAM SETTING PHASE
	CalmUntilFail->childTask = FirstSequence;
	RootTask = CalmUntilFail;
	bIsStopped = false;
	MaterialInstance = UMaterialInstanceDynamic::Create(ownerBT->GetMesh()->GetMaterial(5),
		ownerBT);


}


void UCalmBT::StopTree()
{
	Super::StopTree();
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
}

//Condition
ETaskExeState UCalmBT::IsReachable()
{
	if (NavSys->GetRandomReachablePointInRadius(ownerBT->GetActorLocation(),
		3000.0f, randLocation))
	{
		return ETaskExeState::Success;//Success!
	}
	
	return ETaskExeState::Fail;//Fail !
}

//Action
ETaskExeState UCalmBT::GoToRandPosition()
{
	AIController->MoveToLocation(randLocation.Location);
	return ETaskExeState::Success;//Success !
}







