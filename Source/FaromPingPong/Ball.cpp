// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void ABall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag(FName("Gate")))
	{
		if (OtherActor->ActorHasTag("Player_1"))
		{
			OnGateReached.Broadcast(true);
		}
		else
		{
			OnGateReached.Broadcast(false);
		}
	}
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh)
	{
		Mesh->OnComponentHit.AddDynamic(this, &ABall::OnHit);
	}
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

