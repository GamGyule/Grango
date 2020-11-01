// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GObjectDataStruct.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "GDetailSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS(HideDropdown)
class GRANGO_API UGDetailSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
    void InitValue(FObjectDataStruct DataStruct);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* SlotBtn;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* Icon;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Name;

	UPROPERTY(BlueprintReadOnly)
	FString GObjectRowName;
	
};
