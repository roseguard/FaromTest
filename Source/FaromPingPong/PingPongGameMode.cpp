#include "PingPongGameMode.h"
#include "PlayerPawn.h"
#include <Kismet/GameplayStatics.h>
#include "Ball.h"

void APingPongGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer || !PlayerClass)
        return;

    AActor* SpawnPoint = nullptr;
    if (ConnectedPlayers == 0)
    {
        SpawnPoint = FindPlayerStart(NewPlayer, "Player_1");
    }
    else
    {
        SpawnPoint = FindPlayerStart(NewPlayer, "Player_2");
    }     

    if (SpawnPoint)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        const FVector location = SpawnPoint->GetActorLocation();
        const FRotator rotation = SpawnPoint->GetActorRotation();

        APlayerPawn* NewPawn = Cast<APlayerPawn>(GetWorld()->SpawnActor(
            PlayerClass,
            &location,
            &rotation,
            SpawnParams
        ));
        ensureAlways(NewPawn);

        if (NewPawn)
        {
            NewPlayer->SetAsLocalPlayerController();
            NewPlayer->Possess(NewPawn);

            if (ConnectedPlayers == 0)
            {
                HostPawn = Cast<APlayerPawn>(NewPawn);
                ensureAlways(HostPawn);
            }
            else
            {
                ClientPawn = Cast<APlayerPawn>(NewPawn);
                ensureAlways(ClientPawn);
            }
        }
        ++ConnectedPlayers;
    }
    if (ConnectedPlayers == 2)
    {
        StartGame();
    }
}

void APingPongGameMode::ResetBall()
{
    BallInstance->Destroy(true);
    FActorSpawnParameters spawnParams;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    const FVector location = BallSpawnPoint->GetActorLocation();
    const FRotator rotation = BallSpawnPoint->GetActorRotation();
    BallInstance = Cast<ABall>(GetWorld()->SpawnActor(BallClass, 
        &location, 
        &rotation,
        spawnParams));
    ensureAlways(BallInstance);

    if (BallInstance)
    {
        BallInstance->OnGateReached.AddDynamic(this, &APingPongGameMode::BallInGate);
    }
}

void APingPongGameMode::BallInGate(bool isHostGate)
{
    isHostGate ? ++ClientScore : ++HostScore;
    HostPawn->ClientUpdateScore(HostScore, ClientScore);
    ClientPawn->ClientUpdateScore(HostScore, ClientScore);
    ResetBall();
}

void APingPongGameMode::StartGame()
{
    TArray<AActor*> ballSpawnPoints;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BallSpawn"), ballSpawnPoints);
    if (ballSpawnPoints.Num() == 1)
    {
        BallSpawnPoint = ballSpawnPoints[0];
    }
    else
    {
        return;
    }

    if (BallClass)
    {
        HostScore = 0;
        ClientScore = 0;
        
        FActorSpawnParameters spawnParams;
        spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        const FVector location = BallSpawnPoint->GetActorLocation();
        const FRotator rotation = BallSpawnPoint->GetActorRotation();
        BallInstance = Cast<ABall>(GetWorld()->SpawnActor(BallClass, 
            &location, 
            &rotation, 
            spawnParams)
        );
        ensureAlways(BallInstance);

        if (BallInstance)
        {
            BallInstance->OnGateReached.AddDynamic(this, &APingPongGameMode::BallInGate);
        }

        HostPawn->ServerStartGame();
        ClientPawn->ServerStartGame();
    }
}