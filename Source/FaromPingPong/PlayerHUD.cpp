// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/TextBlock.h"

void UPlayerHUD::SetScore(int hostScore, int clientScore)
{
	if (ScoreBlock)
	{
		ScoreBlock->SetText(FText::FromString(FString::Printf(TEXT("Host : Client\r\n%d:%d"), hostScore, clientScore)));
	}
}

void UPlayerHUD::SetWaitingPopup(bool visible)
{
	if (Waiting)
	{
		if (visible)
		{
			Waiting->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Waiting->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
