#include "ConfirmPurchaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UConfirmPurchaseWidget::Setup(const FText& InSkillName, int32 InCost)
{
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
  OnConfirmed.Broadcast();
  RemoveFromParent();
}

void UConfirmPurchaseWidget::HandleCancelClicked()
{
  RemoveFromParent();
}
