#include "AdvLocFunctionLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"

UAnimSequence* UAdvLocFunctionLibrary::GetDirectionalAnimation(const FAdvLocDirectionalAnimationsPack& DirectionalAnimationsPack,
                                                               const EAdvLocLocomotionDirection Direction, const EAdvLocGait Gait, const EAdvLocStance Stance)
{
	const FAdvLocDirectionalAnimations* Animations;

	switch (Stance)
	{
		case EAdvLocStance::Stand:
			switch (Gait)
			{
				case EAdvLocGait::Walk: Animations = &DirectionalAnimationsPack.WalkDirectionalAnimations; break;
				case EAdvLocGait::Jog: Animations = &DirectionalAnimationsPack.JogDirectionalAnimations; break;
				default: return nullptr;
			} break;
		case EAdvLocStance::Crouch:
			switch (Gait)
			{
				case EAdvLocGait::Walk: Animations = &DirectionalAnimationsPack.CrouchWalkDirectionalAnimations; break;
				case EAdvLocGait::Jog: Animations = &DirectionalAnimationsPack.CrouchJogDirectionalAnimations; break;
				default: return nullptr;
			} break;
		default: return nullptr;
	}

	switch (Direction)
	{
		case EAdvLocLocomotionDirection::Backward: return Animations->BackwardAnimation.Get();
		case EAdvLocLocomotionDirection::Forward: return Animations->ForwardAnimation.Get();
		case EAdvLocLocomotionDirection::Left: return Animations->LeftAnimation.Get();
		case EAdvLocLocomotionDirection::Right: return Animations->RightAnimation.Get();
		default: return nullptr;
	}
}

FAdvLocStopPredictionData UAdvLocFunctionLibrary::GetStopPredictionData(UCharacterMovementComponent* CharacterMovement)
{
	FAdvLocStopPredictionData StopPredictionData = FAdvLocStopPredictionData();
	if (!CharacterMovement) return StopPredictionData;
	
	StopPredictionData.bUseSeparateBrakingFriction = CharacterMovement->bUseSeparateBrakingFriction;
	StopPredictionData.BrakingFriction = CharacterMovement->BrakingFriction;
	StopPredictionData.GroundFriction = CharacterMovement->GroundFriction;
	StopPredictionData.BrakingFrictionFactor = CharacterMovement->BrakingFrictionFactor;
	StopPredictionData.BrakingDecelerationWalking = CharacterMovement->BrakingDecelerationWalking;
	return StopPredictionData;
}
