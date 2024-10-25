// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;

UCLASS()
class FAROMPINGPONG_API APlayerPawn : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* BoxComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed = 1000.0f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnServerLocationChanged)
	FVector ServerLocation;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnGameStarted)
	bool GameStarted = false;
public:
	APlayerPawn();
	UFUNCTION()
	void Moving(const FInputActionValue& value);
	UFUNCTION(Server, UnReliable)
	void ServerMoving(FVector value);
	UFUNCTION()
    void OnServerLocationChanged();
	UFUNCTION()
	void OnGameStarted();
	UFUNCTION(Client, Reliable)
	void ClientUpdateScore(int hostScore, int ClientScore);
	UFUNCTION(Server, Reliable)
	void ServerStartGame();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	
protected:
	FVector MoveDirection = FVector::ZeroVector;
	class UPlayerHUD* HudInstance = nullptr;

	FVector ClientStartPosition;
	FVector ClientTargetPosition;
	float ClientLerpTime;
	float ClientCurrentTime;
};
