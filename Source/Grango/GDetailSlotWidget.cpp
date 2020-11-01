// Fill out your copyright notice in the Description page of Project Settings.


#include "GDetailSlotWidget.h"
#include "GPlayerController.h"

void UGDetailSlotWidget::NativeConstruct()
{
    
}

void UGDetailSlotWidget::InitValue(FObjectDataStruct DataStruct)
{
    Name->SetText(FText::FromString(DataStruct.ObjectName));
    Icon->SetBrushFromTexture(DataStruct.ObjectIcon);
}
