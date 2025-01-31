// Fill out your copyright notice in the Description page of Project Settings.


#include "WHBear.h"

// Sets default values
AWHBear::AWHBear()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BEAR(TEXT("/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP"));
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
}

// Called when the game starts or when spawned
void AWHBear::BeginPlay()
{
	Super::BeginPlay();
	
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

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AWHBear::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AWHBear::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AWHBear::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AWHBear::LookUp);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AWHBear::Jump);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AWHBear::GetOff);
}

//Movement
void AWHBear::MoveForward(float AxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
}
void AWHBear::MoveRight(float AxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
}
void AWHBear::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}
void AWHBear::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

//Ride
void AWHBear::InteractWith()
{
	OriginalPawn = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (OriginalPawn->IsValidLowLevelFast())
	{
		OriginalPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OriginalPawn->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		OriginalPawn->AttachToComponent(Attach, FAttachmentTransformRules::KeepWorldTransform);
		FTransform AttachTransform;
		OriginalPawn->SetActorTransform(Attach->GetComponentTransform());
		GetWorld()->GetFirstPlayerController()->Possess(this);
	}
}
void AWHBear::InRange()
{
	UE_LOG(LogTemp, Warning, TEXT("Bear in range"));
}
void AWHBear::OutRange()
{
	UE_LOG(LogTemp, Warning, TEXT("Bear out range"));
}
void AWHBear::GetOff()
{
	FTransform Transform = Detach->GetComponentTransform();
	OriginalPawn->SetActorTransform(Transform);
	GetWorld()->GetFirstPlayerController()->Possess(OriginalPawn);
}