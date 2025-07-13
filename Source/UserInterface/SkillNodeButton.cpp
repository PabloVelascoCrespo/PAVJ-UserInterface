#include "SkillNodeButton.h"
#include "SkillTreeComponent.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USkillNodeButton::Setup(const FSkillNode& _Node)
{
  ID = _Node.ID;
  NameText->SetText(_Node.Name);
  CostText->SetText(FText::AsNumber(_Node.Cost));

  // This is just to change the button's color depending on the branch
  const FString IDStr = ID.ToString();
  FLinearColor Color = FLinearColor::White;
  if (IDStr.StartsWith("Speed_"))
  {
    Color = FLinearColor(0.f, 0.4f, 1.f);
  }
  else if (IDStr.StartsWith("Health_"))
  {
    Color = FLinearColor(0.f, 0.8f, 0.f);
  }
  else if (IDStr.StartsWith("Damage_"))
  {
    Color = FLinearColor(0.8f, 0.f, 0.f);
  }

  RootButton->SetBackgroundColor(Color);

  // Setting the button enabled or disabled dependingg on the State of the Skill
  switch (_Node.State)
  {
  case ESkillState::Locked:
    RootButton->SetIsEnabled(false);
    break;

  case ESkillState::Unlocked:
    RootButton->SetIsEnabled(true);
    break;

  case ESkillState::Purchased:
    RootButton->SetIsEnabled(false);
    break;

  default:
    // This is just in case someone adds a new state in the ESkillState enum.
    UE_LOG(LogTemp, Warning, TEXT("[SKill Node Button] Not controlled Skill State."));
    break;
  }
}

bool USkillNodeButton::Initialize()
{
  if (!Super::Initialize())
  {
    return false;
  }

  // Setting up the Delegate for when the button is clicked
  RootButton->OnClicked.AddDynamic(this, &USkillNodeButton::HandleRootButtonClicked);
  return true;
}

void USkillNodeButton::HandleRootButtonClicked()
{
  OnClickedDelegate.ExecuteIfBound(ID);
}
