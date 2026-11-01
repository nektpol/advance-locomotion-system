#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "Types/AdvLocTypes.h"
#include "AdvLocAnimationControlComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AdvLocLog, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAdvLocGaitChanged, EAdvLocGait, PreviousGait, EAdvLocGait, NewGait);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAdvLocStanceChanged, EAdvLocStance, PreviousStance, EAdvLocStance, NewStance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAdvLocLocomotionStyleChanged, EAdvLocLocomotionStyle, PreviousStyle, EAdvLocLocomotionStyle, NewStyle);

class UAnimInstance;
class UAdvLocAnimationRegistry;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Animation), meta=(BlueprintSpawnableComponent))
class ADVANCELOCOMOTIONSYSTEM_API UAdvLocAnimationControlComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAdvLocAnimationControlComponent();
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Setup")
    FComponentReference TargetMeshReference;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Registry")
    TObjectPtr<UAdvLocAnimationRegistry> RegistryOverride;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Profile")
    FGameplayTag InitialProfileTag;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Debug")
    bool bEnableDebugMessages = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Ground Detection", meta=(ClampMin="0.0"))
    float MaxGroundDetectionDistance = 1000.f;
    
    UFUNCTION(BlueprintCallable, Category="Animation|Profile")
    bool SetAnimationProfile(FGameplayTag ProfileTag);

    UFUNCTION(BlueprintCallable, Category="Animation|State")
    void SetGait(EAdvLocGait NewGait);

    UFUNCTION(BlueprintCallable, Category="Animation|State")
    void SetStance(EAdvLocStance NewStance);

    UFUNCTION(BlueprintCallable, Category="Animation|State")
    void SetLocomotionStyle(EAdvLocLocomotionStyle NewLocomotionStyle);
    
    UPROPERTY(BlueprintAssignable) FAdvLocGaitChanged OnGaitChanged;
    UPROPERTY(BlueprintAssignable) FAdvLocStanceChanged OnStanceChanged;
    UPROPERTY(BlueprintAssignable) FAdvLocLocomotionStyleChanged OnLocomotionStyleChanged;
    
    UFUNCTION(BlueprintCallable, Category="Animation|State") 
    EAdvLocGait GetGait() const { return Gait; }
    UFUNCTION(BlueprintCallable, Category="Animation|State") 
    EAdvLocStance GetStance() const { return Stance; }
    UFUNCTION(BlueprintCallable, Category="Animation|State") 
    EAdvLocLocomotionStyle GetLocomotionStyle() const { return LocomotionStyle; }
    UFUNCTION(BlueprintCallable, Category="Animation|State") 
    FGameplayTag GetActiveProfileTag() const { return ActiveProfileTag; }
    
    UFUNCTION(BlueprintCallable, Category="Animation|Ground Detection")
    float GetGroundDistance() const { return GroundDistance; }
    UFUNCTION(BlueprintCallable, Category="Animation|Ground Detection")
    bool HasGroundBelow() const { return bHasGroundBelow; }
    UFUNCTION(BlueprintCallable, Category="Animation|Ground Detection")
    bool IsGroundDetectionEnabled() const { return bEnableGroundDetection; }

private:
    UPROPERTY(Transient)
    TObjectPtr<USkeletalMeshComponent> ControlledMesh;

    UPROPERTY(Transient)
    TObjectPtr<UAdvLocAnimationRegistry> ActiveRegistry;

    UPROPERTY(Transient)
    TSubclassOf<UAnimInstance> ActiveFullBodyClass;

    UPROPERTY(Transient)
    TSubclassOf<UAnimInstance> ActiveUpperBodyClass;
    
    EAdvLocGait Gait = EAdvLocGait::Walk;
    EAdvLocStance Stance = EAdvLocStance::Stand;
    EAdvLocLocomotionStyle LocomotionStyle = EAdvLocLocomotionStyle::Directional;
    FGameplayTag ActiveProfileTag;
    
    float GroundDistance = 0.f;
    bool bHasGroundBelow = false;
    bool bEnableGroundDetection = true;
    
    UPROPERTY()
    TSoftObjectPtr<UAdvLocAnimationRegistry> DefaultRegistry;

    void SetSettingsOnOwnerCharacter();
    USkeletalMeshComponent* ResolveTargetMesh() const;
    UAdvLocAnimationRegistry* ResolveRegistry();
    void UnlinkActiveLayers();
    
    UFUNCTION()
    void HandleMovementModeChanged(ACharacter* Character, EMovementMode PreviousMovementMode,uint8 PreviousCustomMode);
    void SetGroundDetectionActive(bool bActive);
    void UpdateGroundDistance();
};
