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
  // it has 2 conditions
  //  1.- That node has as prerequisite the current Node
  //  2.- That node is locked (it makes no sense to unlock an already unlocked node)
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
  // Getting a node by it's ID
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
  // Getting a constant node by it's ID
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

  // Unlocking the Nodes that have no Prerequisites
  for (auto& Node : SkillNodes)
  {
    if (Node.Prerequisites.Num() == 0)
    {
      Node.State = ESkillState::Unlocked;
    }
  }
}