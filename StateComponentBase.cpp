// Fill out your copyright notice in the Description page of Project Settings.


#include "StateComponentBase.h"

// Sets default values for this component's properties
UStateComponentBase::UStateComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//Adding all the possible states by default to Possible Transitions
	UEnum* EnumPtr = FindObject<UEnum>(GetTransientPackage(), TEXT("ECharacterState"), true);
	if (EnumPtr)
	{
		// Get the maximum valid enum value
		const ECharacterState MaxEnumValue = static_cast<ECharacterState>(EnumPtr->GetMaxEnumValue());

		for (int32 EnumIndex = 0; EnumIndex < EnumPtr->NumEnums(); ++EnumIndex)
		{
			ECharacterState EnumValue = static_cast<ECharacterState>(EnumPtr->GetValueByIndex(EnumIndex));

			// Skip the maximum valid enum value. By default UE adds a 'MAX' value to UEnums, which I don't want or need on the list.
			if (EnumValue == MaxEnumValue)
			{
				continue;
			}

			CanTransitionFromStateList.Add(EnumValue, true); // Set the default value to true
		}
	}
	// ...
}


// Called when the game starts
void UStateComponentBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerCapsule = GetOwner()->GetComponentByClass<UCapsuleComponent>();
	PlayerMovement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
	PlayerCharacter = Cast<AMyCharacter>(GetOwner());
	// ...
}


// Called every frame
void UStateComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UStateComponentBase::OnSetStateConditionCheck(UCharacterStateMachine& SM)
{
	return true;
}

void UStateComponentBase::OnEnterState(UCharacterStateMachine& SM)
{
	OnEnterStateDelegate.Broadcast();
	if (!CountTowardsFalling) PlayerCharacter->ResetFalling();
	if (ResetsDash) PlayerCharacter->ResetDash();
}

void UStateComponentBase::OnUpdateState(UCharacterStateMachine& SM)
{
	OnUpdateStateDelegate.Broadcast();
}

void UStateComponentBase::OnExitState(UCharacterStateMachine& SM)
{
	OnExitStateDelegate.Broadcast();
	if (!CountTowardsFalling) PlayerCharacter->ResetFalling();
}

void UStateComponentBase::OverrideMovementInput(UCharacterStateMachine& SM, FVector2d& NewMovementVector)
{
}

void UStateComponentBase::OverrideAcceleration(UCharacterStateMachine& SM, float& NewSpeed)
{
}

void UStateComponentBase::OverrideCameraInput(UCharacterStateMachine& SM, FVector2d& NewRotationVector)
{
}

void UStateComponentBase::OverrideDetectState(UCharacterStateMachine& SM)
{
}

void UStateComponentBase::OverrideDebug()
{
}

#pragma region Helper Methods
bool UStateComponentBase::LineTraceSingle(FHitResult& HitR, const FVector& Start, const FVector& End) const
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(HitR, Start, End, ECC_Visibility, CollisionParams);
}

bool UStateComponentBase::LineTraceSingle(const FVector& Start, const FVector& End) const
{
	FHitResult HitR;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(HitR, Start, End, ECC_Visibility, CollisionParams);
}

FVector UStateComponentBase::RotateVector(const FVector& InVector, const float AngleInDegrees, const float Length)
{
	const FRotator Rotation = FRotator(0.0f, AngleInDegrees, 0.0f);
	const FQuat QuatRotation = FQuat(Rotation);
	FVector RotatedVector = QuatRotation.RotateVector(InVector);
	if (Length != 1)
	{
		RotatedVector.Normalize();
		RotatedVector *= Length;
	}
	return RotatedVector;
}

#pragma endregion
