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
	SprintSpeed = 1000.0f;
	CrouchSpeed = 200.0f;
	

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

}

// Called when the game starts or when spawned
void AWHCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AWHCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AWHCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AWHCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AWHCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AWHCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AWHCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AWHCharacter::Walk);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AWHCharacter::Crouch_);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &AWHCharacter::Walk);
}


//Movement
void AWHCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
}
void AWHCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
}
void AWHCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}
void AWHCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}
void AWHCharacter::Walk()
{
	bIsSprinting = false;
	bIsCrouching = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	UnCrouch();
}
void AWHCharacter::Sprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
void AWHCharacter::Crouch_()
{
	bIsCrouching = true;
	Crouch();
}
