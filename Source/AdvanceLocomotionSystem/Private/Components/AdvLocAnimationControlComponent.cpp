#include "Components/AdvLocAnimationControlComponent.h"

#include "AdvLocAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/AdvLocAnimationRegistry.h"
#include "Data/AdvLocResolvedAnimationProfile.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "UObject/SoftObjectPath.h"

DEFINE_LOG_CATEGORY(AdvLocLog);

UAdvLocAnimationControlComponent::UAdvLocAnimationControlComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

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

    SetAnimationProfile(InitialProfileTag);
}

void UAdvLocAnimationControlComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnlinkActiveLayers();

    ActiveRegistry = nullptr;
    ControlledMesh = nullptr;

    Super::EndPlay(EndPlayReason);
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

