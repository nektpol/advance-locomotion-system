#include "AdvLocAnimInstance.h"

EAdvLocLocomotionDirection UAdvLocAnimInstance::CalculateLocomotionDirection(EAdvLocLocomotionDirection CurrentLocomotionDirection,
	const float CurrentLocomotionAngle, const FAdvLocLocomotionDirectionSettings& Settings)
{
	switch (CurrentLocomotionDirection)
	{
	case EAdvLocLocomotionDirection::Backward:
		if (CurrentLocomotionAngle <= Settings.BackwardMin + Settings.DeadZone || CurrentLocomotionAngle >= Settings.BackwardMax - Settings.DeadZone) return EAdvLocLocomotionDirection::Backward;
		break;

	case EAdvLocLocomotionDirection::Forward:
		if (FMath::IsWithinInclusive(CurrentLocomotionAngle, Settings.ForwardMin - Settings.DeadZone, Settings.ForwardMax + Settings.DeadZone)) return EAdvLocLocomotionDirection::Forward;
		break;

	case EAdvLocLocomotionDirection::Left:
		if (FMath::IsWithin(CurrentLocomotionAngle, Settings.BackwardMin - Settings.DeadZone, Settings.ForwardMin + Settings.DeadZone)) return EAdvLocLocomotionDirection::Left;
		break;

	case EAdvLocLocomotionDirection::Right:
		if (FMath::IsWithin(CurrentLocomotionAngle, Settings.ForwardMax - Settings.DeadZone, Settings.BackwardMax + Settings.DeadZone)) return EAdvLocLocomotionDirection::Right;
		break;
	}

	if (CurrentLocomotionAngle <= Settings.BackwardMin || CurrentLocomotionAngle >= Settings.BackwardMax) return EAdvLocLocomotionDirection::Backward;
	if (FMath::IsWithinInclusive(CurrentLocomotionAngle, Settings.ForwardMin, Settings.ForwardMax)) return EAdvLocLocomotionDirection::Forward;
	if (CurrentLocomotionAngle > 0.f) return EAdvLocLocomotionDirection::Right;

	return EAdvLocLocomotionDirection::Left;
}
