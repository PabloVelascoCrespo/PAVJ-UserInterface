#include "ConfirmPurchaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UConfirmPurchaseWidget::Setup(const FText& InSkillName, int32 InCost)
{
  // Setting up the Message that will pop up when this window appears.
  SkillName = InSkillName;
  Cost = InCost;

  if (ConfirmText)
  {
    ConfirmText->SetText(
      FText::Format(
        NSLOCTEXT("ConfirmPurchase", "Prompt", "Do you really want to purchase {0} for {1} skill points?"),
        SkillName,
        FText::AsNumber(Cost)
      )
    );
  }
}

void UConfirmPurchaseWidget::NativeConstruct()
{
  Super::NativeConstruct();

  // Setting up the delegates of Confiming or Canceling the purchase.
  if (ConfirmButton)
  {
    ConfirmButton->OnClicked.AddDynamic(this, &UConfirmPurchaseWidget::HandleConfirmClicked);
  }

  if (CancelButton)
  {
    CancelButton->OnClicked.AddDynamic(this, &UConfirmPurchaseWidget::HandleCancelClicked);
  }
}

void UConfirmPurchaseWidget::HandleConfirmClicked()
{
  // Broadcast on purchase confirmed and removing the widget
  OnConfirmed.Broadcast();
  RemoveFromParent();
}

void UConfirmPurchaseWidget::HandleCancelClicked()
{
  RemoveFromParent();
}
