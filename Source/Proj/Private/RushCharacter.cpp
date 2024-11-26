// Fill out your copyright notice in the Description page of Project Settings.


#include "RushCharacter.h"

#include "AssetTypeCategories.h"
#include "Core/Abilites/RushAttributeSet.h"
#include "Proj/RushAbilitySystemComponent.h"
#include "Proj/RushGameplayAbility.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Kismet/GameplayStatics.h"
#include "Proj/ProjPlayerController.h"
#include "Proj/UI/RushHud.h"


ARushCharacter::ARushCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAbilitiesInitialized = false; 

	AbilitySystemComponent = CreateDefaultSubobject<URushAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	Attributes = CreateDefaultSubobject<URushAttributeSet>(TEXT("Attributes"));

	NetUpdateFrequency = 30.0f; 
	//Om det inte fungerar så är det fel här troligtvis
	// if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	// {
	// 	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	// 	{
	// 		Subsystem->AddMappingContext(MappingContext, 0);
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Failed to find controller"));
	// }
}



// Called to bind functionality to input
void ARushCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetBinds();
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARushCharacter::Move);
		EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::BasicAttack);
		EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::SpecialAttack);
		EnhancedInputComponent->BindAction(UltimateAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::UltimateAttack);
		EnhancedInputComponent->BindAction(BossAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::BossAttack);
		EnhancedInputComponent->BindAction(TauntAction, ETriggerEvent::Triggered, this, &ARushCharacter::Taunt);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ARushCharacter::Dash);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	
}

void ARushCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
	OnMove();
}

void ARushCharacter::BasicAttack()
{
	OnBasicAttack();
}

void ARushCharacter::SpecialAttack()
{
	
	
		OnSpecialAttack();	
	
}

void ARushCharacter::UltimateAttack()
{
	OnUltimateAttack();
}

void ARushCharacter::BossAttack()
{
	OnBossAttack();
}

void ARushCharacter::Taunt()
{
	OnTaunt();
}

void ARushCharacter::Dash()
{
	OnDash();
}


FActiveGameplayEffectHandle ARushCharacter::AddPassiveEffect(const TSubclassOf<UGameplayEffect>& Effect)
{
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle GameplayEffectSpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContextHandle);

	if(GameplayEffectSpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent );
		return ActiveGameplayEffectHandle;
	}
	return NULL;
}

void ARushCharacter::FlushMovementData(UCharacterMovementComponent* MovComp)
{
	MovComp->FlushServerMoves();
}

void ARushCharacter::AddActiveAbility(const TSubclassOf<URushGameplayAbility>& Ability) 
{
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
	Ability, 1,static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
}

void ARushCharacter::RemoveActiveAbility()
{
	//AbilitySystemComponent->RemoveActiveEffects()
}

void ARushCharacter::RemovePassiveAbility(const FActiveGameplayEffectHandle EffectHandle,  const int AmountToRemove) const
{
	const bool F = AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle, AmountToRemove);
}

FGameplayTagContainer ARushCharacter::GetPlayerTags()
{
	FGameplayTagContainer Container;
	AbilitySystemComponent->GetOwnedGameplayTags(Container);
	return Container;
}

void ARushCharacter::OnHealthChanged_Implementation(float DeltaValue, const FGameplayTagContainer& EventTags)
{
}


void ARushCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if(bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ARushCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, ARushCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser); 
}


void ARushCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// server gas init
	if(AbilitySystemComponent)
	{
		AddStartupGameplayAbilities();
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
	SetOwner(NewController);
	InitHud();
}

void ARushCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	SetBinds();
	//InitHud(); Kanske ifall multiplayer senare?
}


void ARushCharacter::DisplayGameOverWidget_Implementation()
{
	if(GameOverMenu == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No gameover widget added to player"));
		return; 
	}

	GameOverMenu->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("Widget added to viewport"));
}

void ARushCharacter::DisplayGameOverWidgetEvent()
{
	DisplayGameOverWidget();
}

void ARushCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if( GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		// grants abilities on the server 
		
		for( const TSubclassOf<URushGameplayAbility>& Ability : GameplayAbilities )
		{
			AddActiveAbility(Ability);
			AbilitySystemComponent->InitAbilityActorInfo(this,this);
		}
		
		for( const TSubclassOf<UGameplayEffect>& Effect : PassiveGameplayEffects )
		{
			AddPassiveEffect(Effect);
			AbilitySystemComponent->InitAbilityActorInfo(this,this);
		}
		bAbilitiesInitialized = true;
	}
}


void ARushCharacter::RefreshActor()
{
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

UAbilitySystemComponent* ARushCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARushCharacter::CreateGameOverMenu()
{
	if(GameOverMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No gameover widget added to player"));
		return; 
	}

	if(GameOverMenu != nullptr)
	{

		UE_LOG(LogTemp, Warning, TEXT("A game over widget has already been created"));
		return; 
	}
	
	APlayerController* controllerCasted = Cast<APlayerController>(GetController());
	
	GameOverMenu = CreateWidget<UUserWidget>( controllerCasted, GameOverMenuWidget);
	
	if(GameOverMenu == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create widget"));	
		return;
	}
	//GameOverMenu->SetOwningPlayer(ControllerCasted);

	//GameOverMenu->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("Spawned a widget"));
}


void ARushCharacter::SetBinds() 
{
	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"ERushAbilityInputID",
			static_cast<int32>(ERushAbilityInputID::Confirm),
			static_cast<int32>(ERushAbilityInputID::Cancel)
			);
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}

	AddStartupGameplayAbilities();
}

void ARushCharacter::InitHud() const
{
	UE_LOG(LogTemp, Error, TEXT("hejsvejs"));
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(ARushHud* RushHud = Cast<ARushHud>(PlayerController->GetHUD()))
		{
			UE_LOG(LogTemp, Error, TEXT("DEN INITIERAS"));
			RushHud->Init();
		}
	}
}

