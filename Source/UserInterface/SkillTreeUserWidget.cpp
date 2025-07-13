#include "SkillTreeUserWidget.h"
#include "Components/VerticalBox.h"
#include "SkillTreeComponent.h"
#include "SkillNodeButton.h"
#include "ConfirmPurchaseWidget.h"
#include "Components/TextBlock.h"

void USkillTreeUserWidget::RefreshTree()
{
  if (!SkillComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("[Skill Tree User Widget] SkillTree Component Not Found"));
    return;
  }

  // Setting up the Remaining Points in the Widget, so the player can now how many points has left
  if (RemainingPointsText)
  {
    RemainingPointsText->SetText(FText::AsNumber(SkillComponent->RemainingPoints));
  }

  // Just in case deleting all the children in the boxes before filling it dynamically
  for (UVerticalBox* BranchBox : BranchBoxes)
  {
    BranchBox->ClearChildren();
  }

  // Dynamically filling the SkillTree, setting up the Skill Buttons and binding the delegate for when the Node is being purchased
  for (const FSkillNode& Node : SkillComponent->SkillNodes)
  {
    int32 BranchIndex = Node.BranchIndex;
    USkillNodeButton* Button = CreateWidget<USkillNodeButton>(this, NodeButtonClass);
    Button->Setup(Node);
    Button->OnClickedDelegate.BindLambda([this](FName NodeID)
      {
        OnNodeConfirmed(NodeID);
      });
    BranchBoxes[BranchIndex]->AddChild(Button);
  }
}

void USkillTreeUserWidget::OnNodeConfirmed(const FName& _NodeID)
{
  // Just in case the node has been unlocked already
  const FSkillNode* Node = SkillComponent->GetNode(_NodeID);
  if (!Node || Node->State != ESkillState::Unlocked && GetOwningPlayer())
  {
    FeedBackText->SetText(FText::FromString("This node has not been unlocked yet!"));
    return;
  }

  // Popping up the Modal window so the player can confirm if he wants the skill or not
  UConfirmPurchaseWidget* Modal = CreateWidget<UConfirmPurchaseWidget>(this, ConfirmWidgetClass);
  Modal->Setup(Node->Name, Node->Cost);
  Modal->OnConfirmed.AddLambda([this, _NodeID]()
    {
      bool bOk = SkillComponent->Purchase(_NodeID);
      RefreshTree();
      if (!bOk && GetOwningPlayer())
      { // In case the player doesn't have enough points showing a message explaining why he didn't get the skill
        FeedBackText->SetText(FText::FromString("You don't have enough points!"));
      }
    });
  Modal->AddToViewport();
}

void USkillTreeUserWidget::NativeConstruct()
{
  Super::NativeConstruct();
 
  // Setting up the Widget
  BranchBoxes.Add(BranchBox_0);
  BranchBoxes.Add(BranchBox_1);
  BranchBoxes.Add(BranchBox_2);

  if (APlayerController* PC = GetOwningPlayer())
  {
    if (APawn* P = PC->GetPawn())
    {
      SkillComponent = P->FindComponentByClass<USkillTreeComponent>();
    }
  }
  RefreshTree();
}
