// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMain.h"

// Sets default values
ABaseMain::ABaseMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ABaseMain::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FVector> ABaseMain::GetCharacterBones()
{
	for(int i = 0; i < bones.Num(); i++)
	{
		bones[i] = GetMesh()->GetBoneLocation(bonesNames[i], EBoneSpaces::WorldSpace);
	}
	return bones;
}

void ABaseMain::SetupBones()
{
	for (int i = 0; i < bonesNames.Num(); i++)
	{
		bones.Add(GetMesh()->GetBoneLocation(bonesNames[i], EBoneSpaces::WorldSpace));
	}
}

// Called every frame
void ABaseMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

