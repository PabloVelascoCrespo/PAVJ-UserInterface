
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeUserWidget.generated.h"

class USkillTreeComponent;
class UVerticalBox;
class USkillNodeButton;
class UConfirmPurchaseWidget;
class UTextBlock;

UCLASS()
class USERINTERFACE_API USkillTreeUserWidget : public UUserWidget
{
  GENERATED_BODY()
public:
  UFUNCTION(BlueprintCallable)
  void RefreshTree();

  void OnNodeConfirmed(const FName& NodeID);

protected:
  virtual void NativeConstruct() override;

  UPROPERTY()
  USkillTreeComponent* SkillComponent;

  TArray<UVerticalBox*> BranchBoxes;

  // The branches are Hardcoded, to add a new branch we have to add a new VerticalBox
  // and add it into the BranchBoxes in the Native Construct
  UPROPERTY(meta = (BindWidget))
  UVerticalBox* BranchBox_0;

  UPROPERTY(meta = (BindWidget))
  UVerticalBox* BranchBox_1;

  UPROPERTY(meta = (BindWidget))
  UVerticalBox* BranchBox_2;

  UPROPERTY(meta = (BindWidget))
  UTextBlock* RemainingPointsText;

  UPROPERTY(meta = (BindWidget))
  UTextBlock* FeedBackText;


  UPROPERTY(EditDefaultsOnly, Category = "SkillTree|UI")
  TSubclassOf<USkillNodeButton> NodeButtonClass;

  UPROPERTY(EditAnywhere)
  TSubclassOf<UConfirmPurchaseWidget> ConfirmWidgetClass;
};
