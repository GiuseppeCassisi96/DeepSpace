// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainEnemy.h"



// Sets default values
AMainEnemy::AMainEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AlfredAI = CreateDefaultSubobject<UAlfred>(TEXT("AlfredAI"));
	AlfredFSM = CreateDefaultSubobject<UAlfredFSM>(TEXT("AlfredFSM"));
	CalmBT = NewObject<UCalmBT>();
	AttackBT = NewObject<UAttackBT>();
	ViewBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewBox"));
	ViewBox->SetupAttachment(RootComponent);
	HearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HearingSphere"));
	HearingSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainEnemy::BeginPlay()
{
	Super::BeginPlay();
	CalmBT->InitTree(this, FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()));
	AlfredFSM->GetStates().Add(EEnemyState::Calm, CalmBT);
	AlfredFSM->RunActionOfCurrentState();
	
}

// Called every frame
void AMainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




