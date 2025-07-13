#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillNodeButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnSkillNodeClicked, FName);

struct FSkillNode;
class UButton;
class UTextBlock;

UCLASS()
class USERINTERFACE_API USkillNodeButton : public UUserWidget
{
  GENERATED_BODY()
public:
  void Setup(const FSkillNode& _Node);

  virtual bool Initialize() override;

  UFUNCTION()
  void HandleRootButtonClicked();

  FOnSkillNodeClicked OnClickedDelegate;

protected:
  UPROPERTY(meta = (BindWidget))
  UButton* RootButton;

  UPROPERTY(meta = (BindWidget))
  UTextBlock* NameText;

  UPROPERTY(meta = (BindWidget))
  UTextBlock* CostText;

  FName ID;
};
