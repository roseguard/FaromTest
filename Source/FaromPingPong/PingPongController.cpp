// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

void APingPongController::SetupInputComponent()
{
	Super::SetupInputComponent();

	MappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	UInputTrigger* downTrigger = NewObject<UInputTriggerDown>(this);
	UInputTrigger* releaseTrigger = NewObject<UInputTriggerReleased>(this);
	MoveAction->Triggers.Add(downTrigger);
	MoveAction->Triggers.Add(releaseTrigger);
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	MappingContext->MapKey(MoveAction, EKeys::W);

	FEnhancedActionKeyMapping& BackMapping = MappingContext->MapKey(MoveAction, EKeys::S);
	UInputModifier* Negate = NewObject<UInputModifierNegate>(this);
	UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(this);
	BackMapping.Modifiers.Add(Negate);

	FEnhancedActionKeyMapping& LeftMapping = MappingContext->MapKey(MoveAction, EKeys::A);
	LeftMapping.Modifiers.Add(Swizzle);
	LeftMapping.Modifiers.Add(Negate);

	FEnhancedActionKeyMapping& RightMapping = MappingContext->MapKey(MoveAction, EKeys::D);
	RightMapping.Modifiers.Add(Swizzle);
}
