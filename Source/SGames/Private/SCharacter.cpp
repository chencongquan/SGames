// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../SGames.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"
#include "SInteractionComponent.h"
#include "SGameModeBase.h"
//#include "InputTriggers.h"



// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	//SpringArmComp->AttachToComponent();这个通常用于游戏中的动态添加
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 800.f;
	//SpringArmComp->SetRelativeRotation(FRotator(-30, 0, 0));//没用 因为用的control的rotation
	SpringArmComp->bUsePawnControlRotation = true;
	// We control the rotation of spring arm with pawn control rotation already, this disables a subtle side effect
	// where rotating our CapsuleComponent (eg. caused by bOrientRotationToMovement in Character Movement) will rotate our spring arm until it self corrects later in the update
	// This may cause unwanted effects when using CameraLocation during Tick as it may be slightly offset from our final camera position.
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;


	GetCharacterMovement()->bOrientRotationToMovement = true; // 向运动方向转身
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0); //转身速度
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .2f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	Subsystem->AddMappingContext(DefaultInputMapping, 0);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	InputComp->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &ASCharacter::LookMouse);

	InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	InputComp->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);

	// Sprint while key is held
	InputComp->BindAction(Input_Sprint, ETriggerEvent::Started, this, &ASCharacter::SprintStart);
	InputComp->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ASCharacter::SprintStop);

	// Abilities
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
	InputComp->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::BlackHoleAttack);
	InputComp->BindAction(Input_Dash, ETriggerEvent::Triggered, this, &ASCharacter::Dash);
}

void ASCharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

	//方法一
	AddMovementInput(ControlRot.Quaternion().GetForwardVector(), AxisValue.Y);

	AddMovementInput(ControlRot.Quaternion().GetRightVector(), AxisValue.X);

	//方法二
	//AddMovementInput(ControlRot.Vector(), AxisValue.Y);
	//// 获取相机（鼠标控制器）的朝向，转向右侧，并朝这个方向移动；传入的Y表示右侧
	//const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	//AddMovementInput(RightVector, AxisValue.X);
}

void ASCharacter::LookMouse(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ASCharacter::PrimaryInteract()
{
	LogOnScreen(this, TEXT("Interact"));
	InteractionComp->PrimaryInteract();
}

void ASCharacter::SprintStart()
{
	LogOnScreen(this, TEXT("SprintStart"));
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	LogOnScreen(this, TEXT("SprintStop"));
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
	LogOnScreen(this, TEXT("PrimaryAttack"));
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackHoleAttack()
{
	LogOnScreen(this, TEXT("BlackHoleAttack"));
	ActionComp->StartActionByName(this, "Blackhole");
}

void ASCharacter::Dash()
{
	LogOnScreen(this, TEXT("Dash"));
	ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// Died
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		//DisableInput(PC);
		//不知道为啥 这里必须要调用Unpossess，不然Restart Player不生效
		PC->UnPossess();
		// ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetLifeSpan(5.0f);
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM && PC)
		{
			GM->RestartPlayer(PC);
		}
	}
}

