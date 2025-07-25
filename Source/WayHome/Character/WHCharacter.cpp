// Fill out your copyright notice in the Description page of Project Settings.


#include "WHCharacter.h"

// Sets default values
AWHCharacter::AWHCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsSprinting = false;
	bIsCrouching = false;
	WalkSpeed = 500.0f;
	SprintSpeed = 700.0f;
	CrouchSpeed = 300.0f;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHILD(TEXT("/Game/Assets/Character/Player/CharacterTheBoy.CharacterTheBoy"));
	if (SK_CHILD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHILD.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Game/Assets/Character/Player/Animation/AB_TheBoy.AB_TheBoy_C"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bDoCollisionTest = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	//Interaction
	Interaction = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction"));
	Interaction->SetSphereRadius(150.0f);
	Interaction->SetupAttachment(RootComponent);
	Interaction->SetCollisionProfileName(TEXT("Interaction"));

	//Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>CONTEXT(TEXT("/Game/Blueprints/Character/Input/IMC_Character.IMC_Character"));
	if (CONTEXT.Succeeded())
		CharacterMappingContext = CONTEXT.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE(TEXT("/Game/Blueprints/Character/Input/IA_Move.IA_Move"));
	if (IA_MOVE.Succeeded())
		MoveInputAction = IA_MOVE.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK(TEXT("/Game/Blueprints/Character/Input/IA_Look.IA_Look"));
	if (IA_LOOK.Succeeded())
		LookInputAction = IA_LOOK.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP(TEXT("/Game/Blueprints/Character/Input/IA_Jump.IA_Jump"));
	if (IA_JUMP.Succeeded())
		JumpInputAction = IA_JUMP.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SPRI(TEXT("/Game/Blueprints/Character/Input/IA_Sprint.IA_Sprint"));
	if (IA_SPRI.Succeeded())
		SpriInputAction = IA_SPRI.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CROU(TEXT("/Game/Blueprints/Character/Input/IA_Crouch.IA_Crouch"));
	if (IA_CROU.Succeeded())
		CrouInputAction = IA_CROU.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_INTE(TEXT("/Game/Blueprints/Character/Input/IA_Interact.IA_Interact"));
	if (IA_INTE.Succeeded())
		InteInputAction = IA_INTE.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DASH(TEXT("/Game/Blueprints/Character/Input/IA_Dash.IA_Dash"));
	if (IA_DASH.Succeeded())
		DashInputAction = IA_DASH.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_UITO(TEXT("/Game/Blueprints/Character/Input/IA_UIToggle.IA_UIToggle"));
	if (IA_UITO.Succeeded())
		UIToInputAction = IA_UITO.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Asce(TEXT("/Game/Blueprints/Character/Input/IA_Ascend.IA_Ascend"));
	if (IA_Asce.Succeeded())
		AsceInputAction = IA_Asce.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DESC(TEXT("/Game/Blueprints/Character/Input/IA_Descend.IA_Descend"));
	if (IA_DESC.Succeeded())
		DescInputAction = IA_DESC.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_GIMM(TEXT("/Game/Blueprints/Character/Input/IA_Gimmick.IA_Gimmick"));
	if (IA_GIMM.Succeeded())
		GimmInputAction = IA_GIMM.Object;

	bCanSprint = true;
	bCanCrouch = true;
	bCanJump = true;
	bCanAscend = false;
	bCanDescend = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Abilities"));
}

// Called when the game starts or when spawned
void AWHCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}
}
void AWHCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}
}

// Called every frame
void AWHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Interaction
	TArray<AActor*>OverlappingActors;
	Interaction->GetOverlappingActors(OverlappingActors);

	//If Nothing is in range
	if (OverlappingActors.Num() == 0)
	{
		if (InteractableActor)
		{
			if (InteractableActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				IInteractionInterface::Execute_OutRange(InteractableActor);
			}
			InteractableActor = nullptr;
		}
		return;
	}

	//Get closest actor
	AActor* Closest = OverlappingActors[0];
	for (auto CurrentActor : OverlappingActors)
	{
		if (GetDistanceTo(CurrentActor) < GetDistanceTo(Closest))
		{
			Closest = CurrentActor;
		}
	}
	
	if (InteractableActor && InteractableActor != Closest)
	{
		if (InteractableActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			IInteractionInterface::Execute_OutRange(InteractableActor);
		}
	}
	if (InteractableActor != Closest)
	{
		InteractableActor = Closest;
		if (InteractableActor && InteractableActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			IInteractionInterface::Execute_InRange(InteractableActor);
		}
	}
}

// Called to bind functionality to input
void AWHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AWHCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AWHCharacter::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &AWHCharacter::Jump_);
		EnhancedInputComponent->BindAction(SpriInputAction, ETriggerEvent::Triggered, this, &AWHCharacter::Sprint);
		EnhancedInputComponent->BindAction(SpriInputAction, ETriggerEvent::Completed, this, &AWHCharacter::Walk);
		EnhancedInputComponent->BindAction(CrouInputAction, ETriggerEvent::Triggered, this, &AWHCharacter::Crouch_);
		EnhancedInputComponent->BindAction(CrouInputAction, ETriggerEvent::Completed, this, &AWHCharacter::Walk);
		EnhancedInputComponent->BindAction(AsceInputAction, ETriggerEvent::Triggered, this, &AWHCharacter::Ascend);
		EnhancedInputComponent->BindAction(DescInputAction, ETriggerEvent::Triggered, this, &AWHCharacter::Descend);
		EnhancedInputComponent->BindAction(InteInputAction, ETriggerEvent::Started, this, &AWHCharacter::Interact);
		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Started, this, &AWHCharacter::DashCharge);
		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Completed, this, &AWHCharacter::Dash);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AWHCharacter::DoubleJump);
		EnhancedInputComponent->BindAction(SpriInputAction, ETriggerEvent::Started, this, &AWHCharacter::Glide);
		EnhancedInputComponent->BindAction(SpriInputAction, ETriggerEvent::Completed, this, &AWHCharacter::EndGlide);
		EnhancedInputComponent->BindAction(GimmInputAction, ETriggerEvent::Started, this, &AWHCharacter::Gimmik);
	}

	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AWHCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AWHCharacter::MoveRight);
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AWHCharacter::Turn);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AWHCharacter::LookUp);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AWHCharacter::Jump);
	//PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AWHCharacter::Sprint);
	//PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AWHCharacter::Walk);
	//PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AWHCharacter::Crouch_);
	//PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &AWHCharacter::Walk);
	//PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AWHCharacter::Interact);
}

//Movement
//void AWHCharacter::MoveForward(float AxisValue)
//{
//	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
//}
//void AWHCharacter::MoveRight(float AxisValue)
//{
//	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
//}
//void AWHCharacter::Turn(float AxisValue)
//{
//	AddControllerYawInput(AxisValue);
//}
//void AWHCharacter::LookUp(float AxisValue)
//{
//	AddControllerPitchInput(AxisValue);
//}
void AWHCharacter::Walk()
{
	if(bCanSprint && bCanCrouch)
	{
		bIsSprinting = false;
		bIsCrouching = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		UnCrouch();
	}
}
void AWHCharacter::Sprint()
{
	if(bCanSprint)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void AWHCharacter::Crouch_()
{
	if(bCanCrouch && GetCharacterMovement()->IsMovingOnGround())
	{
		bIsCrouching = true;
		Crouch();
	}
}

void AWHCharacter::Jump_()
{
	if (bCanJump)
	{
		Jump();
	}
}

void AWHCharacter::Move(const FInputActionValue& Value)
{
	FVector2D AxisValue = Value.Get<FVector2D>();
	if(GetController())
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;
		AddMovementInput(FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X), AxisValue.Y);
		AddMovementInput(FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y), AxisValue.X);
	}
}
void AWHCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(AxisValue.X);
		AddControllerPitchInput(AxisValue.Y * -1.0f);
	}
}

void AWHCharacter::Ascend(const FInputActionValue& Value)
{
	if (bCanAscend)
	{
		float AxisValue = Value.Get<float>();
		FVector ForwardVector = GetActorForwardVector();
		ForwardVector = ForwardVector * 0.00001f;
		AddMovementInput(FVector(0.0f, 0.0f, 1.0f) + ForwardVector, AxisValue);
	}
}
void AWHCharacter::Descend(const FInputActionValue& Value)
{
	if (bCanDescend)
	{
		float AxisValue = Value.Get<float>();
		FVector ForwardVector = GetActorForwardVector();
		ForwardVector = ForwardVector * 0.00001f;
		AddMovementInput(FVector(0.0f, 0.0f, -1.0f) + ForwardVector, AxisValue);
	}
}

//Interaction
void AWHCharacter::Interact()
{
	if(InteractableActor)
	{
		if (InteractableActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			IInteractionInterface::Execute_InteractWith(InteractableActor);
		}
	}
}
void AWHCharacter::Gimmik()
{
	OnGimmikInteract.Broadcast();
}

//Abilities
UAbilitySystemComponent* AWHCharacter::GetAbilityComponent()
{
	return AbilitySystemComponent;
}

//Dash
void AWHCharacter::DashCharge()
{
	//AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Dash"))));
	AbilitySystemComponent->PressInputID(0);
}
void AWHCharacter::Dash()
{
	AbilitySystemComponent->ReleaseInputID(0);
}

//DoubleJump
void AWHCharacter::DoubleJump()
{
	if (GetCharacterMovement()->IsFalling())
	{
		AbilitySystemComponent->PressInputID(1);
	}
}
void AWHCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	AbilitySystemComponent->ReleaseInputID(1); //For DoubleJump
	AbilitySystemComponent->ReleaseInputID(2); //For Gliding
}

//Glide
void AWHCharacter::Glide()
{
	if (GetCharacterMovement()->IsFalling())
	{
		AbilitySystemComponent->PressInputID(2);
	}
}
void AWHCharacter::EndGlide()
{
	AbilitySystemComponent->ReleaseInputID(2);
}
