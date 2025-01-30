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
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

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

	//Character
	Attach = CreateDefaultSubobject<USceneComponent>(TEXT("Attach"));
	Detach = CreateDefaultSubobject<USceneComponent>(TEXT("Detach"));
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHILD(TEXT("/Game/Assets/Character/Player/CharacterTheBoy.CharacterTheBoy"));
	if (SK_CHILD.Succeeded())
	{
		CharacterMesh->SetSkeletalMesh(SK_CHILD.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Game/Assets/Character/Player/Animation/AB_TheBoy.AB_TheBoy_C"));
	if (ANIM.Succeeded())
	{
		CharacterMesh->SetAnimInstanceClass(ANIM.Class);
	}

	Attach->SetupAttachment(GetCapsuleComponent());
	Detach->SetupAttachment(GetCapsuleComponent());
	CharacterMesh->SetupAttachment(Attach);

	Attach->SetRelativeLocation(FVector(0.0f, -40.0f, 0.0f));
	Detach->SetRelativeLocation(FVector(90.0f, 0.0f, -70.0f));

	CharacterMesh->SetVisibility(false, true);
	GetMesh()->SetCollisionProfileName(TEXT("InteractableBlock"));
	GetMesh()->SetGenerateOverlapEvents(true);
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

void AWHBear::GetOff()
{
	FTransform Transform = Detach->GetComponentTransform();
	OriginalPawn->SetActorTransform(Transform);
	GetWorld()->GetFirstPlayerController()->Possess(OriginalPawn);
}

//Ride
void AWHBear::InteractWith()
{
	OriginalPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	GetWorld()->GetFirstPlayerController()->Possess(this);
	CharacterMesh->SetVisibility(true, true);
}
void AWHBear::InRange()
{
	UE_LOG(LogTemp, Warning, TEXT("Bear in range"));
}
void AWHBear::OutRange()
{
	UE_LOG(LogTemp, Warning, TEXT("Bear out range"));
}
