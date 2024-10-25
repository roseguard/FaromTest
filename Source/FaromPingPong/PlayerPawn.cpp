// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <MovieSceneSequencePlayer.h>
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Net/UnrealNetwork.h>
#include <Camera/CameraComponent.h>
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerHUD.h"
#include "PingPongController.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->Deactivate();

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HUD"));
	WidgetComponent->SetupAttachment(CameraComponent);
}

void APlayerPawn::Moving(const FInputActionValue& value)
{
	FVector2D direction = value.Get<FVector2D>();
	FVector mvDirection = FVector(direction.X, direction.Y, 0);
	ServerMoving(mvDirection);
}

void APlayerPawn::ServerMoving_Implementation(FVector direction)
{
	MoveDirection = direction;
}

void APlayerPawn::OnServerLocationChanged()
{
	SetActorLocation(ServerLocation);
}

void APlayerPawn::OnGameStarted()
{
	if (HudInstance && GameStarted)
	{
		HudInstance->SetWaitingPopup(false);
	}
	else if (GameStarted)
	{
		UUserWidget* userWidget = WidgetComponent->GetWidget();
		if (userWidget)
		{
			HudInstance = Cast<UPlayerHUD>(userWidget);
			if (HudInstance)
			{
				HudInstance->SetWaitingPopup(false);
			}
		}
	}
}

void APlayerPawn::ClientUpdateScore_Implementation(int hostScore, int clientScore)
{
	if (HudInstance)
	{
		HudInstance->SetScore(hostScore, clientScore);
	}
}

void APlayerPawn::ServerStartGame_Implementation()
{
	GameStarted = true;
	if (HudInstance && GameStarted)
	{
		HudInstance->SetWaitingPopup(false);
	}
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	CameraComponent->Activate(true);

	if (!HudInstance)
	{
		UUserWidget* userWidget = WidgetComponent->GetWidget();
		if (userWidget)
		{
			HudInstance = Cast<UPlayerHUD>(userWidget);
		}
	}

	if (HudInstance && GameStarted)
	{
		HudInstance->SetWaitingPopup(false);
	}
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APingPongController* PlayerController = Cast<APingPongController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerController->MappingContext, 0);
		}
	}

	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent && PlayerController)
	{
		enhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Moving);
	}
}

void APlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerPawn, ServerLocation);
	DOREPLIFETIME(APlayerPawn, GameStarted);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MoveDirection.IsZero() && GameStarted && GetNetMode() == NM_ListenServer)
	{
		FHitResult* sweepResult = new FHitResult();
		AddActorLocalOffset(MoveDirection * Speed * DeltaTime, true, sweepResult, ETeleportType::None);
		if (sweepResult && sweepResult->bBlockingHit && sweepResult->GetActor()->IsRootComponentMovable())
		{
			AddActorLocalOffset(MoveDirection * Speed * DeltaTime, false, nullptr, ETeleportType::None);
		}
		ServerLocation = GetActorLocation();		
	}
}

