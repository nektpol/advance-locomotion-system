#include "Components/AdvLocAnimationControlComponent.h"

#include "AdvLocAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/AdvLocAnimationRegistry.h"
#include "Data/AdvLocResolvedAnimationProfile.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/SoftObjectPath.h"

DEFINE_LOG_CATEGORY(AdvLocLog);

UAdvLocAnimationControlComponent::UAdvLocAnimationControlComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    DefaultRegistry = TSoftObjectPtr<UAdvLocAnimationRegistry>(FSoftObjectPath(TEXT("/AdvanceLocomotionSystem/Data/DA_AdvLoc_DefaultRegistry.DA_AdvLoc_DefaultRegistry")));
}

void UAdvLocAnimationControlComponent::BeginPlay()
{
    Super::BeginPlay();

    ControlledMesh = ResolveTargetMesh();

    if (!IsValid(ControlledMesh))
    {
        if (bEnableDebugMessages && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(100, 10.f, FColor::Red, 
                FString::Printf(TEXT("UAdvLocAnimationControlComponent::BeginPlay: No valid Skeletal Mesh found on %s."), *GetNameSafe(GetOwner())));
        }
        UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::BeginPlay: No valid Skeletal Mesh found on %s."), *GetNameSafe(GetOwner()));
        return;
    }
    SetSettingsOnOwnerCharacter();

    if (!IsValid(ControlledMesh->GetAnimInstance()))
    {
        if (bEnableDebugMessages && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(101, 10.f, FColor::Red, 
                FString::Printf(TEXT("UAdvLocAnimationControlComponent::BeginPlay: Skeletal Mesh on %s has no active Animation Blueprint."), *GetNameSafe(GetOwner())));
        }
        UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::BeginPlay: Skeletal Mesh on %s has no active Animation Blueprint."), *GetNameSafe(GetOwner()));
        return;
    }
    UAdvLocAnimInstance* AnimInstance = Cast<UAdvLocAnimInstance>(ControlledMesh->GetAnimInstance());

    if (IsValid(AnimInstance))
    {
        AnimInstance->SetAnimationControlComponent(this);
    }

    ActiveRegistry = ResolveRegistry();

    if (!IsValid(ActiveRegistry))
    {
        if (bEnableDebugMessages && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(102, 10.f, FColor::Red, 
                FString::Printf(TEXT("UAdvLocAnimationControlComponent::BeginPlay: No valid Animation Registry found for %s."), *GetNameSafe(GetOwner())));
        }
        UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::BeginPlay: No valid Animation Registry found for %s."), *GetNameSafe(GetOwner()));
        return;
    }
    
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        Character->MovementModeChangedDelegate.AddUniqueDynamic(this, &UAdvLocAnimationControlComponent::HandleMovementModeChanged);

        if (const UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement())
        {
            SetGroundDetectionActive(CharacterMovement->IsFalling());
        }
    }

    SetAnimationProfile(InitialProfileTag);
}

void UAdvLocAnimationControlComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnlinkActiveLayers();

    ActiveRegistry = nullptr;
    ControlledMesh = nullptr;
    
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        Character->MovementModeChangedDelegate.RemoveDynamic(
            this,
            &UAdvLocAnimationControlComponent::HandleMovementModeChanged
        );
    }

    SetComponentTickEnabled(false);

    Super::EndPlay(EndPlayReason);
}

void UAdvLocAnimationControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    UpdateGroundDistance();
}


void UAdvLocAnimationControlComponent::SetSettingsOnOwnerCharacter()
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner)) return;
    
    APawn* Pawn = Cast<APawn>(Owner);
    if (!IsValid(Pawn)) return;

    Pawn->bUseControllerRotationYaw = true;
}

USkeletalMeshComponent* UAdvLocAnimationControlComponent::ResolveTargetMesh() const
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner)) return nullptr;

    const bool bHasReference = !TargetMeshReference.ComponentProperty.IsNone() || !TargetMeshReference.PathToComponent.IsEmpty() || TargetMeshReference.OverrideComponent.IsValid() || TargetMeshReference.OtherActor.IsValid();

    if (bHasReference)
    {
        USkeletalMeshComponent* SelectedMesh = Cast<USkeletalMeshComponent>(TargetMeshReference.GetComponent(Owner));

        if (IsValid(SelectedMesh)) return SelectedMesh;

        UE_LOG(AdvLocLog, Warning, TEXT("UAdvLocAnimationControlComponent::ResolveTargetMesh: Invalid mesh reference on %s. Using fallback."), *Owner->GetName());
    }

    if (ACharacter* Character = Cast<ACharacter>(Owner))
    {
        if (IsValid(Character->GetMesh())) return Character->GetMesh();
    }

    return Owner->FindComponentByClass<USkeletalMeshComponent>();
}

UAdvLocAnimationRegistry* UAdvLocAnimationControlComponent::ResolveRegistry()
{
    if (IsValid(RegistryOverride)) return RegistryOverride.Get();

    return DefaultRegistry.LoadSynchronous();
}


void UAdvLocAnimationControlComponent::UnlinkActiveLayers()
{
    if (IsValid(ControlledMesh))
    {
        if (ActiveUpperBodyClass)
        {
            ControlledMesh->UnlinkAnimClassLayers(ActiveUpperBodyClass);
        }

        if (ActiveFullBodyClass && ActiveFullBodyClass != ActiveUpperBodyClass)
        {
            ControlledMesh->UnlinkAnimClassLayers(ActiveFullBodyClass);
        }
    }

    ActiveFullBodyClass = nullptr;
    ActiveUpperBodyClass = nullptr;
    ActiveProfileTag = FGameplayTag();
}

void UAdvLocAnimationControlComponent::HandleMovementModeChanged(ACharacter* Character, EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    if (!IsValid(Character)) return;

    const UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
    if (!IsValid(CharacterMovement)) return;

    SetGroundDetectionActive(CharacterMovement->IsFalling());
}

void UAdvLocAnimationControlComponent::SetGroundDetectionActive(bool bActive)
{
    bEnableGroundDetection = bActive;
    SetComponentTickEnabled(bEnableGroundDetection);

    if (bEnableGroundDetection)
    {
        UpdateGroundDistance();
    }
    else
    {
        GroundDistance = 0.f;
        bHasGroundBelow = false;
    }
}


void UAdvLocAnimationControlComponent::UpdateGroundDistance()
{
    GroundDistance = MaxGroundDetectionDistance;
    bHasGroundBelow = false;

    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!IsValid(Character)) return;

    const UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
    const UCapsuleComponent* Capsule = Character->GetCapsuleComponent();

    if (!IsValid(CharacterMovement) || !IsValid(Capsule)) return;

    const float TraceDistance = FMath::Max(0.f, MaxGroundDetectionDistance);
    if (TraceDistance <= 0.f) return;

    FFindFloorResult FloorResult;

    CharacterMovement->ComputeFloorDist(
        Capsule->GetComponentLocation(),
        TraceDistance,
        TraceDistance,
        FloorResult,
        Capsule->GetScaledCapsuleRadius()
    );

    if (!FloorResult.IsWalkableFloor()) return;

    GroundDistance = FMath::Clamp(FloorResult.GetDistanceToFloor(), 0.f, TraceDistance);
    bHasGroundBelow = true;
}



bool UAdvLocAnimationControlComponent::SetAnimationProfile(FGameplayTag ProfileTag)
{
    if (!IsValid(ControlledMesh) || !IsValid(ActiveRegistry)) return false;

    if (!IsValid(ControlledMesh->GetAnimInstance()))
    {
        if (bEnableDebugMessages && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(104, 10.f, FColor::Red, 
                FString::Printf(TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Cannot apply a profile without an active Animation Blueprint.")));
        }
        UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Cannot apply a profile without an active Animation Blueprint."));
        return false;
    }

    FAdvLocResolvedAnimationProfile ResolvedProfile;

    if (!ActiveRegistry->ResolveProfile(ProfileTag, ResolvedProfile))
    {
        if (bEnableDebugMessages && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(105, 10.f, FColor::Red, 
                FString::Printf(TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Failed to resolve animation profile %s."), *ProfileTag.ToString()))   ;
        }
        UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Failed to resolve animation profile %s."), *ProfileTag.ToString());
        return false;
    }

    if (ResolvedProfile.FullBodyLayerClass.IsNull())
    {
        if (bEnableDebugMessages && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(106, 10.f, FColor::Red, 
                FString::Printf(TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Profile %s has no Full Body implementation."), *ResolvedProfile.SelectedProfileTag.ToString()));
        }
        UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Profile %s has no Full Body implementation."), *ResolvedProfile.SelectedProfileTag.ToString());
        return false;
    }

    TSubclassOf<UAnimInstance> NewFullBodyClass = ResolvedProfile.FullBodyLayerClass.LoadSynchronous();

    if (!NewFullBodyClass)
    {
        if (bEnableDebugMessages && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(108, 10.f, FColor::Red, 
                FString::Printf(TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Failed to load Full Body class for %s."), *ResolvedProfile.SelectedProfileTag.ToString()));
        }
        UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Failed to load Full Body class for %s."), *ResolvedProfile.SelectedProfileTag.ToString());
        return false;
    }

    TSubclassOf<UAnimInstance> NewUpperBodyClass = nullptr;

    if (!ResolvedProfile.UpperBodyLayerClass.IsNull())
    {
        NewUpperBodyClass = ResolvedProfile.UpperBodyLayerClass.LoadSynchronous();

        if (!NewUpperBodyClass)
        {
            if (bEnableDebugMessages && GEngine)
            {
                GEngine->AddOnScreenDebugMessage(107, 10.f, FColor::Red, 
                    FString::Printf(TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Failed to load Upper Body class for %s."), *ResolvedProfile.SelectedProfileTag.ToString()));
            }
            UE_LOG(AdvLocLog, Error, TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Failed to load Upper Body class for %s."), *ResolvedProfile.SelectedProfileTag.ToString());
            return false;
        }
    }

    if (ActiveProfileTag == ResolvedProfile.SelectedProfileTag && ActiveFullBodyClass == NewFullBodyClass && ActiveUpperBodyClass == NewUpperBodyClass)
    {
        return true;
    }

    UnlinkActiveLayers();

    ControlledMesh->LinkAnimClassLayers(NewFullBodyClass);

    if (NewUpperBodyClass && NewUpperBodyClass != NewFullBodyClass)
    {
        ControlledMesh->LinkAnimClassLayers(NewUpperBodyClass);
    }

    ActiveFullBodyClass = NewFullBodyClass;
    ActiveUpperBodyClass = NewUpperBodyClass;
    ActiveProfileTag = ResolvedProfile.SelectedProfileTag;

    if (bEnableDebugMessages && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(108, 10.f, FColor::Green, 
            FString::Printf(TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Activated animation profile %s on %s."), *ActiveProfileTag.ToString(), *GetNameSafe(GetOwner())));
    }
    UE_LOG(AdvLocLog, Log, TEXT("UAdvLocAnimationControlComponent::SetAnimationProfile: Activated animation profile %s on %s."), *ActiveProfileTag.ToString(), *GetNameSafe(GetOwner()));

    return true;
}

void UAdvLocAnimationControlComponent::SetGait(EAdvLocGait NewGait)
{
    if (Gait == NewGait) return;

    const EAdvLocGait PreviousGait = Gait;
    Gait = NewGait;

    OnGaitChanged.Broadcast(PreviousGait, NewGait);
}

void UAdvLocAnimationControlComponent::SetStance(EAdvLocStance NewStance)
{
    if (Stance == NewStance) return;

    const EAdvLocStance PreviousStance = Stance;
    Stance = NewStance;

    OnStanceChanged.Broadcast(PreviousStance, NewStance);
}

void UAdvLocAnimationControlComponent::SetLocomotionStyle(EAdvLocLocomotionStyle NewLocomotionStyle)
{
    if (LocomotionStyle == NewLocomotionStyle) return;

    const EAdvLocLocomotionStyle PreviousStyle = LocomotionStyle;
    LocomotionStyle = NewLocomotionStyle;

    OnLocomotionStyleChanged.Broadcast(PreviousStyle, NewLocomotionStyle);
}

