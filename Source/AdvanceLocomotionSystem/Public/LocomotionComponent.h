

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/WeaponType.h"
#include "LocomotionComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCELOCOMOTIONSYSTEM_API ULocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULocomotionComponent();
	
	UFUNCTION(BlueprintCallable)
	void InitializeLocomotionComponent();
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentWeaponType(EWeaponType NewWeaponType);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Locomotion|Defaults", meta=(ToolTip="The anim instance class to use for the locomotion layers"))
	TSubclassOf<UAnimInstance> LayersAnimInstance;
	
	UPROPERTY(EditDefaultsOnly, Category="Locomotion|Defaults", meta=(ToolTip="The anim instance class to use for each weapon type layer"))
	TMap<EWeaponType, TSubclassOf<UAnimInstance>> WeaponTypeAnimLayers;

private:
	EWeaponType CurrentWeaponType;
	
	ACharacter* GetOwnerCharacter() const;
	void LinkAnimClassLayers(TSubclassOf<UAnimInstance> AnimClass) const;
};
