// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class FAROMPINGPONG_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetScore(int hostScore, int clientScore);
	UFUNCTION()
	void SetWaitingPopup(bool visible);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Waiting;
};
