#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmPurchaseWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPurchaseConfirmed);

class UTextBlock;
class UButton;

UCLASS()
class USERINTERFACE_API UConfirmPurchaseWidget : public UUserWidget
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, Category = "Confirm")
  void Setup(const FText& InSkillName, int32 InCost);

  FOnPurchaseConfirmed OnConfirmed;

protected:
  virtual void NativeConstruct() override;

  UPROPERTY(meta = (BindWidget))
  UTextBlock* ConfirmText;

  UPROPERTY(meta = (BindWidget))
  UButton* ConfirmButton;

  UPROPERTY(meta = (BindWidget))
  UButton* CancelButton;

private:
  FText SkillName;

  int32 Cost;

  UFUNCTION()
  void HandleConfirmClicked();

  UFUNCTION()
  void HandleCancelClicked();
};
