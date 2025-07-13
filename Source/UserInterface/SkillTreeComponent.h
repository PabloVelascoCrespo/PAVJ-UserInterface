#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillState : uint8
{
  Locked,
  Unlocked,
  Purchased
};
/*
 Since the skills that are unlocked might not apply any change to the character
 I've decided to make the Skill Node a struct, since it will make everything
 easier and faster. If i was trying to applicate this Skill Tree to my game,
 this would be a class in which I'd have virtual mehtods that would be overwritten
 by the subclasses, those virtual methods would be the effects of unlocking this skill
*/
USTRUCT(BlueprintType)
struct FSkillNode
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  FName ID;

  UPROPERTY(EditAnywhere)
  FText Name;

  UPROPERTY(EditAnywhere)
  int32 Cost;

  /**
   * @brief Branch index is a number between 0-2 indicating which branch does this node belong to
   * This should be something like Category: Speed, Damage or Health... But to simplify it I made it like this
   */
  UPROPERTY(EditAnywhere)
  int32 BranchIndex;

  UPROPERTY(EditAnywhere)
  TArray<FName> Prerequisites;

  UPROPERTY(EditAnywhere)
  ESkillState State = ESkillState::Locked;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USERINTERFACE_API USkillTreeComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  USkillTreeComponent();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
  int32 RemainingPoints = 5;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
  TArray<FSkillNode> SkillNodes;

  bool CanPurchase(const FName& _ID) const;

  bool Purchase(const FName& _ID);

  FSkillNode* GetNode(const FName& _ID);
  const FSkillNode* GetNode(const FName& _ID) const;

protected:
  virtual void BeginPlay() override;
};
