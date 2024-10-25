// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameMode.generated.h"

/*
*/
UCLASS()
class FAROMPINGPONG_API APingPongGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	AActor* BallSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	TSubclassOf<AActor> BallClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TSubclassOf<AActor> PlayerClass;
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION()
	void BallInGate(bool isHostGate);
protected:
	void StartGame();
	void ResetBall();
protected:
	int ConnectedPlayers = 0;
	class ABall* BallInstance = nullptr;
	int HostScore;
	int ClientScore;

	class APlayerPawn* HostPawn = nullptr;
	class APlayerPawn* ClientPawn = nullptr;
};
