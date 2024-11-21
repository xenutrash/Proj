// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjPlayerController.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "RushCharacter.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjPlayerController::AProjPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// We could use later? 
	//CachedDestination = FVector::ZeroVector;
	//FollowTime = 0.f;
}

void AProjPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AProjPlayerController::SetupInputComponent()
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
	
	
		// Setup Keyboard input events
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjPlayerController::Move);
	
		// Mouse click input
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AProjPlayerController::RotateCharacterTowardsClick);
	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



// WASD movement
void AProjPlayerController::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn(); // Get the controlled pawn
	if (ControlledPawn != nullptr)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();
		
		// find out which way is forward
		const FRotator Rotation = ControlledPawn->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

void AProjPlayerController::RotateCharacterTowardsClick()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FHitResult Hit;
		bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

		if (bHitSuccessful)
		{
			FVector TargetLocation = Hit.Location;
			FVector PawnLocation = ControlledPawn->GetActorLocation();

			// Calculate the direction vector from the pawn to the target location
			FVector Direction = (TargetLocation - PawnLocation).GetSafeNormal2D();

			// Convert the direction to a rotation
			FRotator NewRotation = Direction.Rotation();

			// Apply the rotation to the character (only Yaw)
			ControlledPawn->SetActorRotation(FRotator(0, NewRotation.Yaw, 0));
		}
	}
}

void AProjPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	UE_LOG(LogTemp, Warning, TEXT("Acknowledged"));
	ARushCharacter* CharacterBase = Cast<ARushCharacter>(P);
	if(CharacterBase)
	{
		CharacterBase->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterBase,CharacterBase);
	}
}





