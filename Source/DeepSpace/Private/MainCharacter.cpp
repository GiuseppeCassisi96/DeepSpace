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
	USkeletalMeshComponent* CharMesh = GetMesh();
	int32 index = CharMesh->GetBoneIndex("Spine");
	spineTrasform = CharMesh->GetBoneSpaceTransforms()[index];
	AnimInstance = CharMesh->GetAnimInstance();
	springArmLenght = SpringArm->TargetArmLength;
	startMovementSpeed = movementSpeed;
	bones.Add(GetMesh()->GetBoneLocation("Head", EBoneSpaces::WorldSpace));
	bones.Add(GetMesh()->GetBoneLocation("LeftShoulder", EBoneSpaces::WorldSpace));
	bones.Add(GetMesh()->GetBoneLocation("RightShoulder", EBoneSpaces::WorldSpace));
	bones.Add(GetMesh()->GetBoneLocation("Spine1", EBoneSpaces::WorldSpace));
	bones.Add(GetMesh()->GetBoneLocation("LeftLeg", EBoneSpaces::WorldSpace));
	bones.Add(GetMesh()->GetBoneLocation("RightLeg", EBoneSpaces::WorldSpace));
	OnTakeAnyDamage.AddDynamic(this, &AMainCharacter::TakeDamageFromEnemy);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	forwardVelocity = GetMovementComponent()->Velocity.Length();
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
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::Crouch);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMainCharacter::Aim);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMainCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMainCharacter::Run);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &AMainCharacter::Throw);
	}
}

void AMainCharacter::Move(const FInputActionValue& actionValue)
{
	FVector2D inputValue = actionValue.Get<FVector2D>();
	FVector forwardDirection = GetActorForwardVector();
	FVector rightDirection = GetActorRightVector();
	FVector direction = forwardDirection * inputValue.X + rightDirection * inputValue.Y;
	rightMovementValue = inputValue.Y;
	forwardMovementValue = inputValue.X;

	//Right movement
	if(inputValue.Y > 0.0f)
	{
		isMovementRight = true;
		isMovementLeft = false;
		SetState(AnimState::Walk);
		if(isCrouch)
		{
			isMovementRight = false;
			isCrouchMovementRight = true;
			isCrouchMovementLeft = false;
			SetState(AnimState::Crouch);
		}
	}
	else if (inputValue.Y < 0.0f)
	{
		SetState(AnimState::Walk);
		isMovementRight = false;
		isMovementLeft = true;
		if (isCrouch)
		{
			SetState(AnimState::Crouch);
			isMovementLeft = false;
			isCrouchMovementRight = false;
			isCrouchMovementLeft = true;
		}
	}
	else
	{
		isMovementRight = false;
		isMovementLeft = false;
		isCrouchMovementRight = false;
		isCrouchMovementLeft = false;
	}

	//Forward movement
	if(inputValue.X < 0)
	{
		SetState(AnimState::Walk);
		isMovementBack = true;
		if (isCrouch)
		{
			SetState(AnimState::Crouch);
			isMovementBack = false;
			isCrouchMovementBack = true;
		}
	}
	else if(inputValue.X > 0)
	{
		SetState(AnimState::Walk);
		isMovementBack = false;
		isCrouchMovementBack = false;
		if (isCrouch)
		{
			SetState(AnimState::Crouch);
		}
	}
	else
	{
		isMovementBack = false;
		isCrouchMovementBack = false;
	}
	FVector movement = direction * movementSpeed;
	AddMovementInput(movement);
}

void AMainCharacter::Rotation(const FInputActionValue& actionValue)
{
	FVector2D inputValue = actionValue.Get<FVector2D>();
	SpringArm->AddLocalRotation(FRotator(inputValue.Y * rotationSpeed, 0.0f, 0.0f));
	AddControllerYawInput(inputValue.X * rotationSpeed);

	//Clamp rotation
	FRotator currentRotation = SpringArm->GetRelativeRotation();
	float pitch = FMath::Clamp(currentRotation.Pitch, -50.0f, 20.0f);
	SpringArm->SetRelativeRotation(FRotator(pitch, currentRotation.Yaw, currentRotation.Roll));
	if(isAiming)
	{
		spineRotation = FRotator(0.0f, 0.0f,
			-pitch );
	}
	
}

void AMainCharacter::Crouch(const FInputActionValue& actionValue)
{
	isCrouch = !isCrouch;
	isMovementRight = false;
	isMovementLeft = false;
	isCrouchMovementRight = false;
	isCrouchMovementLeft = false;
	isCrouchMovementBack = false;
}

void AMainCharacter::Aim(const FInputActionValue& actionValue)
{
	isAiming = !isAiming;
	AnimInstance->Montage_JumpToSection("Aim");
	AnimInstance->Montage_Play(aimMontage);
	SpringArm->TargetArmLength = 50.0f;
	if(!isAiming)
	{
		AnimInstance->Montage_Stop(0.0f, aimMontage);
		spineRotation = FRotator(0.0f, 0.0f,
			3.63f);
		SpringArm->TargetArmLength = springArmLenght;
	}
}

void AMainCharacter::Run(const FInputActionValue& actionValue)
{
	if(actionValue.Get<bool>() && forwardMovementValue > 0 && rightMovementValue == 0 && !isCrouch)
	{
		movementSpeed =  1.7f;
		SetState(AnimState::Run);
	}
	else
	{
		movementSpeed = startMovementSpeed;
		SetState(AnimState::Run);
	}
}

void AMainCharacter::Throw(const FInputActionValue& actionValue)
{
	FVector location = GetActorLocation() + GetActorForwardVector() * 100.0f;
	FRotator rotation = FRotator::ZeroRotator;
	AThrowableItem* obj = Cast<AThrowableItem>(GetWorld()->SpawnActor(ThrowableObj,&location,&rotation));
	obj->ItemMesh->AddForce(300000.0f * GetActorForwardVector());
}

TArray<FVector> AMainCharacter::GetCharacterBones()
{
	return Super::GetCharacterBones();
}

void AMainCharacter::SetState(AnimState newState)
{
	state = newState;
}

void AMainCharacter::TakeDamageFromEnemy(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DamageCauser->GetActorLabel());
}





