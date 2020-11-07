// Fill out your copyright notice in the Description page of Project Settings.


#include "GGameWidget.h"

#include "GDetailSlotWidget.h"
#include "GPlayerController.h"


void UGGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GGameInstance = Cast<UAdvancedGameInstance>(GetGameInstance());
    
    BtnMake->OnClicked.AddDynamic(this,&UGGameWidget::BtnMakeOnClicked);

    ScaleInput->OnValueChanged.AddDynamic(this,&UGGameWidget::SpinValueUpdate);
    OctInput->OnValueChanged.AddDynamic(this,&UGGameWidget::SpinValueUpdate);
    PerInput->OnValueChanged.AddDynamic(this,&UGGameWidget::SpinValueUpdate);
    LacInput->OnValueChanged.AddDynamic(this,&UGGameWidget::SpinValueUpdate);
    SeedInput->OnValueChanged.AddDynamic(this,&UGGameWidget::SpinValueUpdate);
    OffsetXInput->OnValueChanged.AddDynamic(this,&UGGameWidget::SpinValueUpdate);
    OffsetYInput->OnValueChanged.AddDynamic(this,&UGGameWidget::SpinValueUpdate);
}

void UGGameWidget::BtnMakeOnClicked()
{
    PanelVisible(MakingPanel, ESlateVisibility::Visible);
    PanelVisible(MenuPanel,ESlateVisibility::Hidden);
    LoadDetailGrid();
}

void UGGameWidget::SpinValueUpdate(float Value)
{
    auto GPlayerController = Cast<AGPlayerController>(GetOwningPlayer());
    GPlayerController->TestPerlin();
}

void UGGameWidget::PanelVisible(UPanelWidget* Panel, ESlateVisibility V)
{
    Panel->SetVisibility(V);
    if(V == ESlateVisibility::Visible)
    {
        OpenPanels.Add(Panel);
    }else
    {
        if(OpenPanels.Contains(Panel))
        {
            OpenPanels.Remove(Panel);
        }
    }
}

void UGGameWidget::LoadDetailGrid()
{
    TArray<FName> OutRowNames = GGameInstance->GObjectDataTable->GetRowNames();
    int Column = 0, Row = 0;
    for(int i = 0; i < OutRowNames.Num(); i++)
    {
        UGDetailSlotWidget* SlotWidget = CreateWidget<UGDetailSlotWidget>(GetOwningPlayer(),DetailSlot);
        FObjectDataStruct rows = GGameInstance->GObjectData(OutRowNames[i].ToString());
        SlotWidget->GObjectRowName = OutRowNames[i].ToString();
        SlotWidget->InitValue(rows);

        if(i % 2 == 0)
        {
            DetailGrid->AddChildToUniformGrid(SlotWidget,Column,Row);
            Column++;
        }else
        {
            Column = 0;
            Row++;
            DetailGrid->AddChildToUniformGrid(SlotWidget,Column,Row);
        }
        
    }
}
