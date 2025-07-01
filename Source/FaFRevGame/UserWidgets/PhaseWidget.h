// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "PhaseWidget.generated.h"

class UButton;
class UTextBlock;
class UExprTextBlock;

UCLASS(Abstract)
class FAFREVGAME_API UPhaseNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class UPhaseWidget;

public:

	UPhaseNodeWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (GetOptions = "GetPhaseNames"))
		FString PhaseName;
#endif

	UPROPERTY(EditAnywhere, Category = Settings)
		FGuid PhaseID;

	UFUNCTION(BlueprintImplementableEvent)
		void MarkSelected(const bool bSelected);

protected:

	UPROPERTY(Transient) TObjectPtr<UPhaseWidget> ParentWidget;
	UPROPERTY(Transient) TObjectPtr<class UGamePhaseGraph> PhaseGraph;

	UGamePhaseGraph* GetGraph();
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
#if WITH_EDITOR
	TArray<FString> GetPhaseNames();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

UCLASS(Abstract)
class FAFREVGAME_API UPhaseWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UPhaseWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> PlayButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> InfoAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Settings)
		TObjectPtr<UUserWidget> ParentUI;

	UFUNCTION(BlueprintImplementableEvent)
		TArray<UPhaseNodeWidget*> GetNodeWidgets();

	void SelectPhase(const FGuid& InPhaseID);

protected:

	UPROPERTY() FGuid SelectedPhase;
	UPROPERTY(Transient) TObjectPtr<UGamePhaseGraph> PhaseGraph;

	UFUNCTION() void OnPlayClicked();
	UFUNCTION() void OnExitClicked() { DeactivateWidget(); }

	virtual void InitWidget() override;
	virtual void InternalProcessDeactivation() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnLaunchMap(this); }
};
