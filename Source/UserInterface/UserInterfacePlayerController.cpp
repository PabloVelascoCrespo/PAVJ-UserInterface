// Copyright Epic Games, Inc. All Rights Reserved.

#include "UserInterfacePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UserInterfaceCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "SkillTreeUserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AUserInterfacePlayerController::AUserInterfacePlayerController()
{
  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Default;
  CachedDestination = FVector::ZeroVector;
  FollowTime = 0.f;
}

void AUserInterfacePlayerController::ToggleSkillTree()
{
  if (SkillTreeWidget && SkillTreeWidget->IsInViewport())
  {
    SkillTreeWidget->RemoveFromViewport();
    SkillTreeWidget = nullptr;

    SetPause(false);

    FInputModeGameOnly GameInput;
    SetInputMode(GameInput);
  }
  else
  {
    SkillTreeWidget = CreateWidget<USkillTreeUserWidget>(this, SkillTreeWidgetClass);
    SkillTreeWidget->AddToViewport();

    SetPause(true);


    FInputModeUIOnly UIMode;
    UIMode.SetWidgetToFocus(SkillTreeWidget->TakeWidget());
    UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(UIMode);
  }
}

void AUserInterfacePlayerController::BeginPlay()
{
  // Call the base class  
  Super::BeginPlay();
}

void AUserInterfacePlayerController::SetupInputComponent()
{
  // set up gameplay key bindings
  Super::SetupInputComponent();

  // Add Input Mapping Context
  if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
  {
    Subsystem->AddMappingContext(DefaultMappingContext, 0);
  }

  // Set up action bindings
  if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
  {
    // Setup mouse input events
    EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AUserInterfacePlayerController::OnInputStarted);
    EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AUserInterfacePlayerController::OnSetDestinationTriggered);
    EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AUserInterfacePlayerController::OnSetDestinationReleased);
    EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AUserInterfacePlayerController::OnSetDestinationReleased);

    // Setup touch input events
    EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AUserInterfacePlayerController::OnInputStarted);
    EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AUserInterfacePlayerController::OnTouchTriggered);
    EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AUserInterfacePlayerController::OnTouchReleased);
    EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AUserInterfacePlayerController::OnTouchReleased);

    EnhancedInputComponent->BindAction(ToggleSkillTreeAction, ETriggerEvent::Started, this, &AUserInterfacePlayerController::ToggleSkillTree);
  }
  else
  {
    UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
  }
}

void AUserInterfacePlayerController::OnInputStarted()
{
  StopMovement();
}

// Triggered every frame when the input is held down
void AUserInterfacePlayerController::OnSetDestinationTriggered()
{
  // We flag that the input is being pressed
  FollowTime += GetWorld()->GetDeltaSeconds();

  // We look for the location in the world where the player has pressed the input
  FHitResult Hit;
  bool bHitSuccessful = false;
  if (bIsTouch)
  {
    bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
  }
  else
  {
    bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
  }

  // If we hit a surface, cache the location
  if (bHitSuccessful)
  {
    CachedDestination = Hit.Location;
  }

  // Move towards mouse pointer or touch
  APawn* ControlledPawn = GetPawn();
  if (ControlledPawn != nullptr)
  {
    FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
    ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
  }
}

void AUserInterfacePlayerController::OnSetDestinationReleased()
{
  // If it was a short press
  if (FollowTime <= ShortPressThreshold)
  {
    // We move there and spawn some particles
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
  }

  FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AUserInterfacePlayerController::OnTouchTriggered()
{
  bIsTouch = true;
  OnSetDestinationTriggered();
}

void AUserInterfacePlayerController::OnTouchReleased()
{
  bIsTouch = false;
  OnSetDestinationReleased();
}
