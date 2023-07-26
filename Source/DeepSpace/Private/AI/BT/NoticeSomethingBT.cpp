// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/NoticeSomethingBT.h"

#include "NavigationPath.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

void UNoticeSomethingBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<AAlfredAIController> AIController)
{
	Super::InitTree(owner, navSys, AIController);
	//INSTANCIES CREATION PHASE: Here I create the node instances
		//TASKS
	TaskCanReach = NewObject<UTaskBT>(this, FName("FTNoticeSomethingBT"));
	TaskGoToLocation = NewObject<UTaskBT>(this, FName("STNoticeSomethingBT"));
		//COMPOSITE TASKS
	FirstSequence = NewObject<USequenceBT>(this, FName("FSNoticeSomethingBT"));

	//BINDING PHASE: Here I bind the task with the function
	TaskCanReach->Task.BindUFunction(this, TEXT("CanReach"));
	TaskGoToLocation->Task.BindUFunction(this, TEXT("GoToLocation"));

	//ADDING PHASE: Here I add the task to the sequence array
	FirstSequence->Tasks.Add(TaskCanReach);
	FirstSequence->Tasks.Add(TaskGoToLocation);

	//PARAM SETTING PHASE
	RootTask = FirstSequence;
	bIsStopped = false;
	MaterialInstance = UMaterialInstanceDynamic::Create(ownerBT->GetMesh()->GetMaterial(5),
		ownerBT);
}

ETaskExeState UNoticeSomethingBT::RunTree()
{
	if (!bIsStopped)
	{
		FLinearColor colorViola(0.51f, 0.0f, 0.51f);
		MaterialInstance->SetVectorParameterValue(TEXT("ColorLight"), colorViola);
		ownerBT->GetMesh()->SetMaterial(5, MaterialInstance);
		ownerBT->GetMesh()->SetMaterial(8, MaterialInstance);
		ownerBT->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		TreeExeState = RootTask->RunTask();
		return TreeExeState;
	}
	TreeExeState = ETaskExeState::Stopped;
	return ETaskExeState::Stopped;
}

void UNoticeSomethingBT::StopTree()
{
	Super::StopTree();
}

ETaskExeState UNoticeSomethingBT::CanReach()
{
	
	if(NavSysBT->FindPathToLocationSynchronously(ownerBT->GetWorld(), 
		ownerBT->GetActorLocation(), SourceLocation))
	{
		return ETaskExeState::Success;
	}
	return ETaskExeState::Fail;

}

ETaskExeState UNoticeSomethingBT::GoToLocation()
{
	AlfredAIController->MoveToLocation(SourceLocation);
	return ETaskExeState::Success;
}
