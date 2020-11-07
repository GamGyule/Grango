// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "AdvancedGameInstance.h"
#include "GDetailSlotWidget.h"
#include "Components/SpinBox.h"


#include "GGameWidget.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class GRANGO_API UGGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, *Msg);
	}

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UImage* NoiseImage;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	USpinBox* ScaleInput;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	USpinBox* OctInput;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	USpinBox* PerInput;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	USpinBox* LacInput;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	USpinBox* SeedInput;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	USpinBox* OffsetXInput;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	USpinBox* OffsetYInput;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UPanelWidget*> OpenPanels;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UCanvasPanel* MenuPanel;

	UPROPERTY(meta=(BindWidget))
	UButton* BtnMake;

	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UCanvasPanel* MakingPanel;
	
	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UUniformGridPanel* CategoryGrid;
	
	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UUniformGridPanel* DetailGrid;
	
	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UCanvasPanel* BuildHelpPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGDetailSlotWidget> DetailSlot;

	UFUNCTION(BlueprintCallable)
    void PanelVisible(UPanelWidget* Panel,ESlateVisibility V);

	UFUNCTION(BlueprintCallable)
    void LoadDetailGrid();

protected:
	UPROPERTY()
	UAdvancedGameInstance* GGameInstance;

	UFUNCTION()
	void BtnMakeOnClicked();

	UFUNCTION()
	void SpinValueUpdate(float Value);


	
	
	
};
