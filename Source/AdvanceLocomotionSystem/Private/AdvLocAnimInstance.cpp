#include "AdvLocAnimInstance.h"

EAdvLocLocomotionDirection UAdvLocAnimInstance::CalculateLocomotionDirection(EAdvLocLocomotionDirection CurrentLocomotionDirection,
	const float VelocityLocomotionAngle, const FAdvLocLocomotionDirectionSettings& Settings)
{
	switch (CurrentLocomotionDirection)
	{
	case EAdvLocLocomotionDirection::Backward:
		if (VelocityLocomotionAngle <= Settings.BackwardMin + Settings.DeadZone || VelocityLocomotionAngle >= Settings.BackwardMax - Settings.DeadZone) return EAdvLocLocomotionDirection::Backward;
		break;

	case EAdvLocLocomotionDirection::Forward:
		if (FMath::IsWithinInclusive(VelocityLocomotionAngle, Settings.ForwardMin - Settings.DeadZone, Settings.ForwardMax + Settings.DeadZone)) return EAdvLocLocomotionDirection::Forward;
		break;

	case EAdvLocLocomotionDirection::Left:
		if (FMath::IsWithin(VelocityLocomotionAngle, Settings.BackwardMin - Settings.DeadZone, Settings.ForwardMin + Settings.DeadZone)) return EAdvLocLocomotionDirection::Left;
		break;

	case EAdvLocLocomotionDirection::Right:
		if (FMath::IsWithin(VelocityLocomotionAngle, Settings.ForwardMax - Settings.DeadZone, Settings.BackwardMax + Settings.DeadZone)) return EAdvLocLocomotionDirection::Right;
		break;
	}

	if (VelocityLocomotionAngle <= Settings.BackwardMin || VelocityLocomotionAngle >= Settings.BackwardMax) return EAdvLocLocomotionDirection::Backward;
	if (FMath::IsWithinInclusive(VelocityLocomotionAngle, Settings.ForwardMin, Settings.ForwardMax)) return EAdvLocLocomotionDirection::Forward;
	if (VelocityLocomotionAngle > 0.f) return EAdvLocLocomotionDirection::Right;

	return EAdvLocLocomotionDirection::Left;
}
