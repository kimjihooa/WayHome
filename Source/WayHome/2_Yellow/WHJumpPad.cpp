// Fill out your copyright notice in the Description page of Project Settings.


#include "WHJumpPad.h"

// Sets default values
AWHJumpPad::AWHJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetCollisionProfileName("OverlapAll");
	Box->OnComponentBeginOverlap.AddDynamic(this, &AWHJumpPad::OnBoxBeginOverlap);
	Box->SetBoxExtent(FVector(32.0f, 32.0f, 5.0f));

	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Dir"));
	Direction->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Direction->SetupAttachment(Box);
	Direction->SetArrowLength(40.0f);

	LunchFactor = 1.0f;
}

// Called when the game starts or when spawned
void AWHJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWHJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWHJumpPad::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if(OtherCharacter->IsValidLowLevelFast())
	{
		FVector Incoming = OtherCharacter->GetVelocity();
		FVector Normal = Direction->GetForwardVector();
		FVector Reflected = Incoming - 2 * FVector::DotProduct(Incoming, Normal) * Normal;

		OtherCharacter->LaunchCharacter(Reflected * LunchFactor, true, true);
	}
}


