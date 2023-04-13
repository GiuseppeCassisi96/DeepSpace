// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ThrowableItem.h"



// Sets default values
AThrowableItem::AThrowableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

}

// Called when the game starts or when spawned
void AThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	ItemMesh->OnComponentHit.AddDynamic(this, &AThrowableItem::OnGeneralHit);
	bIsFirstHit = true;
}

// Called every frame
void AThrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AThrowableItem::OnGeneralHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsFirstHit)
	{
		bIsFirstHit = false;
		OnThrowableItemEvent.Broadcast(Hit.Location, itemNoisiness);
	}
	
}

