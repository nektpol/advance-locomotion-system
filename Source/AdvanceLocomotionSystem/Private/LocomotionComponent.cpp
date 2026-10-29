#include "LocomotionComponent.h"

#include "GameFramework/Character.h"
#include "Interfaces/LocomotionInterface.h"


ULocomotionComponent::ULocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULocomotionComponent::InitializeLocomotionComponent()
{
	if (LayersAnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("LayersAnimInstance is nullptr"));
		return;
	}

	LinkAnimClassLayers(LayersAnimInstance);
	SetCurrentWeaponType(EWeaponType::Unarmed);
}

void ULocomotionComponent::SetCurrentWeaponType(EWeaponType NewWeaponType)
{
	if (CurrentWeaponType == NewWeaponType) return;
	
	CurrentWeaponType = NewWeaponType;
	
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (OwnerCharacter == nullptr) return;
	
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr) return;

	if (AnimInstance->Implements<ULocomotionInterface>())
	{
		ILocomotionInterface::Execute_SetWeaponType(AnimInstance, CurrentWeaponType);
	}

	const TSubclassOf<UAnimInstance>* WeaponTypeAnimLayer = WeaponTypeAnimLayers.Find(CurrentWeaponType);
	if (WeaponTypeAnimLayer && *WeaponTypeAnimLayer)
	{
		LinkAnimClassLayers(*WeaponTypeAnimLayer);
	}
}

ACharacter* ULocomotionComponent::GetOwnerCharacter() const
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr) return nullptr;

	return Cast<ACharacter>(Owner);
}

void ULocomotionComponent::LinkAnimClassLayers(TSubclassOf<UAnimInstance> AnimClass) const
{
	if (AnimClass == nullptr) return;
	const ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (OwnerCharacter == nullptr) return;

	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	if (MeshComponent == nullptr) return;

	MeshComponent->LinkAnimClassLayers(AnimClass);
}


