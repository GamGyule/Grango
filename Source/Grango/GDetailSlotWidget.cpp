// Fill out your copyright notice in the Description page of Project Settings.


#include "GDetailSlotWidget.h"
#include "GPlayerController.h"

void UGDetailSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    SlotBtn->OnClicked.AddDynamic(this,&UGDetailSlotWidget::OnClicked_SlotBtn);
}

void UGDetailSlotWidget::InitValue(FObjectDataStruct DataStruct)
{
    Name->SetText(FText::FromString(DataStruct.ObjectName));
    Icon->SetBrushFromTexture(DataStruct.ObjectIcon);
    Material = DataStruct.ObjectMaterial;
}

void UGDetailSlotWidget::OnClicked_SlotBtn()
{
    
    auto* GPlayerController = Cast<AGPlayerController>(GetOwningPlayer());
    
    if(GPlayerController->bIsBuild)
    {
        if(IsValid(GPlayerController->BuildObject))
        {
            GetWorld()->DestroyActor(GPlayerController->BuildObject);
        }
    }else
    {
        GPlayerController->bIsBuild = true;
    }

    UAdvancedGameInstance* GGameInstance = Cast<UAdvancedGameInstance>(GetGameInstance());
    AGObject* Object = GGameInstance->SpawnGObject(GObjectRowName);
    Object->Material = Material;
    GPlayerController->BuildObject = Object;
    
}
