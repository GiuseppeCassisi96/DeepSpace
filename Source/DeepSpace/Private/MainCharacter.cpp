// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character_Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Character_Spring"));
	if (IsValid(GetRootComponent()))
	{
		SpringArm->SetupAttachment(GetRootComponent());
	}
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		//I'm getting the Enhanced Input Subsystem to use the new input system
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			/*I'm adding my Input Mapping Context (IMC) to subsystem. The IMC contains bindings between
			 *input keys and input actions
			 */
			Subsystem->AddMappingContext(MappingContext, 0);
		}
		bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;
	}
	
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//I'm casting the old input system to the new one. The new input system inherit from the old one
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//I'm binding the input action to a specific function
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(RotationAction, ETriggerEvent::Triggered, this, &AMainCharacter::Rotation);
	}
}

void AMainCharacter::Move(const FInputActionValue& actionValue)
{
	FVector2D inputValue = actionValue.Get<FVector2D>();
	inputValue *= movementSpeed;
	FVector forwardDirection = GetActorForwardVector();
	FVector rightDirection = GetActorRightVector();
	FVector direction = forwardDirection * inputValue.X + rightDirection * inputValue.Y;
	FVector movement = direction * GetWorld()->GetDeltaSeconds();
	AddMovementInput(movement);
}

void AMainCharacter::Rotation(const FInputActionValue& actionValue)
{
	float deltaTime = GetWorld()->GetDeltaSeconds();
	FVector2D inputValue = actionValue.Get<FVector2D>();
	SpringArm->AddLocalRotation(FRotator(inputValue.Y * rotationSpeed * deltaTime, 0.0f, 0.0f));
	AddControllerYawInput(inputValue.X * rotationSpeed * deltaTime);

	//Clamp rotation
	FRotator currentRotation = SpringArm->GetRelativeRotation();
	float pitch = FMath::Clamp(currentRotation.Pitch, -50.0f, 20.0f);
	SpringArm->SetRelativeRotation(FRotator(pitch, currentRotation.Yaw, currentRotation.Roll));
}





