// Fill out your copyright notice in the Description page of Project Settings.


#include "WHBear.h"

// Sets default values
AWHBear::AWHBear()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BEAR(TEXT("/Game/Assets/Character/TeddyBear/TeddyBear_Final.TeddyBear_Final"));
	if (SK_BEAR.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BEAR.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP_AnimBlueprint.TutorialTPP_AnimBlueprint_C"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("InteractableBlock"));
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 90.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bDoCollisionTest = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;

	Attach = CreateDefaultSubobject<USceneComponent>(TEXT("Ride"));
	Attach->SetupAttachment(GetCapsuleComponent());
	Attach->SetRelativeLocation(FVector(0.0f, -40.0f, 70.0f));
	DetachL = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GetOffL"));
	DetachL->SetupAttachment(GetCapsuleComponent());
	DetachL->SetRelativeLocation(FVector(0.0f, -120.0f, 0.0f));
	DetachL->SetCapsuleHalfHeight(88.0f);
	DetachL->SetCapsuleRadius(34.0f);
	DetachL->SetCollisionProfileName(FName("OverlapAll"));
	DetachR = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GetOffR"));
	DetachR->SetupAttachment(GetCapsuleComponent());
	DetachR->SetRelativeLocation(FVector(0.0f, 120.0f, 0.0f));
	DetachR->SetCapsuleHalfHeight(88.0f);
	DetachR->SetCapsuleRadius(34.0f);
	DetachR->SetCollisionProfileName(FName("OverlapAll"));
	
	//Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>CONTEXT(TEXT("/Game/Blueprints/2_Yellow/Bear/Input/IMC_Bear.IMC_Bear"));
	if (CONTEXT.Succeeded())
		InputMappingContext = CONTEXT.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE(TEXT("/Game/Blueprints/2_Yellow/Bear/Input/IA_BearMove.IA_BearMove"));
	if (IA_MOVE.Succeeded())
		MoveInputAction = IA_MOVE.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK(TEXT("/Game/Blueprints/2_Yellow/Bear/Input/IA_BearLook.IA_BearLook"));
	if (IA_LOOK.Succeeded())
		LookInputAction = IA_LOOK.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP(TEXT("/Game/Blueprints/2_Yellow/Bear/Input/IA_BearJump.IA_BearJump"));
	if (IA_JUMP.Succeeded())
		JumpInputAction = IA_JUMP.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_INTE(TEXT("/Game/Blueprints/2_Yellow/Bear/Input/IA_BearInteract.IA_BearInteract"));
	if (IA_INTE.Succeeded())
		InteInputAction = IA_INTE.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DASH(TEXT("/Game/Blueprints/2_Yellow/Bear/Input/IA_BearDash.IA_BearDash"));
	if (IA_DASH.Succeeded())
		DashInputAction = IA_DASH.Object;

	//Dash Ability
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability"));
}

// Called when the game starts or when spawned
void AWHBear::BeginPlay()
{
	Super::BeginPlay();
}
void AWHBear::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void AWHBear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWHBear::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AWHBear::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AWHBear::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &AWHBear::Jump);
		EnhancedInputComponent->BindAction(InteInputAction, ETriggerEvent::Triggered, this, &AWHBear::GetOff);
		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Started, this, &AWHBear::DashCharge);
		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Completed, this, &AWHBear::Dash);
	}

	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AWHBear::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AWHBear::MoveRight);
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AWHBear::Turn);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AWHBear::LookUp);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AWHBear::Jump);
	//PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AWHBear::GetOff);
	//PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AWHBear::StartDashCharge);
	//PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, this, &AWHBear::EndDashCharge);
}

//Movement
//void AWHBear::MoveForward(float AxisValue)
//{
//	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
//}
//void AWHBear::MoveRight(float AxisValue)
//{
//	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
//}
//void AWHBear::Turn(float AxisValue)
//{
//	AddControllerYawInput(AxisValue);
//}
//void AWHBear::LookUp(float AxisValue)
//{
//	AddControllerPitchInput(AxisValue);
//}
void AWHBear::Move(const FInputActionValue& Value)
{
	FVector2D AxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue.Y);
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue.X);
	}
}
void AWHBear::Look(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(AxisValue.X);
		AddControllerPitchInput(AxisValue.Y * -1.0f);
	}
}

UAbilitySystemComponent* AWHBear::GetAbilityComponent()
{
	return AbilitySystemComponent;
}

//Dash
void AWHBear::DashCharge()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->PressInputID(0);
	}
}
void AWHBear::Dash()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ReleaseInputID(0);
	}
}

//Ride
void AWHBear::InteractWith_Implementation()
{
	OriginalPawn = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (OriginalPawn->IsValidLowLevelFast())
	{
		OriginalPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OriginalPawn->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OriginalPawn->GetCharacterMovement()->StopMovementImmediately();
		OriginalPawn->AttachToComponent(Attach, FAttachmentTransformRules::KeepWorldTransform);
		OriginalPawn->SetActorTransform(Attach->GetComponentTransform());
		GetWorld()->GetFirstPlayerController()->Possess(this);
	}
}
void AWHBear::InRange_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Bear in range"));
}
void AWHBear::OutRange_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Bear out range"));
}
void AWHBear::GetOff()
{
	TArray<AActor*> LActorList;
	DetachL->GetOverlappingActors(LActorList);
	int OverlapL = LActorList.Num();
	TArray<AActor*> RActorList;
	DetachR->GetOverlappingActors(RActorList);
	int OverlapR = RActorList.Num();

	if (OriginalPawn->IsValidLowLevel() && GetCharacterMovement()->IsMovingOnGround())
	{
		if (OverlapL == 0)
		{
			OriginalPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			OriginalPawn->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			OriginalPawn->GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->StopMovementImmediately();
			OriginalPawn->SetActorTransform(DetachL->GetComponentTransform());
			OriginalPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			GetWorld()->GetFirstPlayerController()->Possess(OriginalPawn);
		}
		else
		{
			if (OverlapR == 0)
			{
				OriginalPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				OriginalPawn->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				OriginalPawn->GetCharacterMovement()->StopMovementImmediately();
				GetCharacterMovement()->StopMovementImmediately();
				OriginalPawn->SetActorTransform(DetachR->GetComponentTransform());
				OriginalPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				GetWorld()->GetFirstPlayerController()->Possess(OriginalPawn);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No where to get off!!"));
			}
		}
	}
}