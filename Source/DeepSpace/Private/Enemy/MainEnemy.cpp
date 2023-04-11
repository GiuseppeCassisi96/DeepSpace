// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainEnemy.h"

// Sets default values
AMainEnemy::AMainEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AlfredAI = CreateDefaultSubobject<UAlfred>(TEXT("AlfredAI"));
	ViewBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewBox"));
	ViewBox->SetupAttachment(GetMesh(),"HeadSocket");
	AIController = CreateDefaultSubobject<AAIController>(TEXT("AI"));
	
}

// Called when the game starts or when spawned
void AMainEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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



