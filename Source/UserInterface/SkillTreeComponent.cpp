#include "SkillTreeComponent.h"

USkillTreeComponent::USkillTreeComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
}


bool USkillTreeComponent::CanPurchase(const FName& _ID) const
{
  // First we find the node we are looking for
  const FSkillNode* Node = GetNode(_ID);

  // Node can't be purchased if it is not found or it's state is not Unlocked (you can't buy a locked node neither you can buy an already purchased node)
  if (!Node || Node->State != ESkillState::Unlocked)
  {
    return false;
  }

  return RemainingPoints >= Node->Cost;
}

bool USkillTreeComponent::Purchase(const FName& _ID)
{
  // If the node is not avaiable to be purchased, you can't buy it.
  if (!CanPurchase(_ID))
  {
    return false;
  }

  // Finding the Node by it's ID
  FSkillNode* Node = GetNode(_ID);

  // Updating the Remaining Points and Node's state
  RemainingPoints -= Node->Cost;
  Node->State = ESkillState::Purchased;

  // Updating all the dependencies
  // it has 3 conditions
  //  1.- Same Branch (you only can unlock a node if it belongs to the same branch)
  //  2.- That node has as prerequisite the current Node
  //  3.- That node is locked
  for (FSkillNode& Dep : SkillNodes)
  {
    if (Dep.Prerequisites.Contains(_ID) && Dep.State == ESkillState::Locked)
    {
      Dep.State = ESkillState::Unlocked;
    }
  }
  return true;
}

FSkillNode* USkillTreeComponent::GetNode(const FName& _ID)
{
  FSkillNode* Node = SkillNodes.FindByPredicate(
    [&](FSkillNode& N)
    {
      return N.ID == _ID;
    }
  );
  return Node;
}

const FSkillNode* USkillTreeComponent::GetNode(const FName& _ID) const
{
  const FSkillNode* Node = SkillNodes.FindByPredicate(
    [&](FSkillNode& N)
    {
      return N.ID == _ID;
    }
  );
  return Node;
}

void USkillTreeComponent::BeginPlay()
{
  Super::BeginPlay();

  for (auto& Node : SkillNodes)
  {
    if (Node.Prerequisites.Num() == 0)
    {
      Node.State = ESkillState::Unlocked;
    }
  }
}