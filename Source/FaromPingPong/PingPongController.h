// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongController.generated.h"

/**
 * 
 */
UCLASS()
class FAROMPINGPONG_API APingPongController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "_Controlls")
	class UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "_Controlls")
	class UInputAction* MoveAction;
public:
	virtual void SetupInputComponent() override;
};
