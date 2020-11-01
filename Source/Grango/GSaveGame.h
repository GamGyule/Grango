// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GSaveGame.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSaveGameStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int StructInteger;
};

UCLASS()
class GRANGO_API UGSaveGame : public USaveGame
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString SaveSlotName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int SaveIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSaveGameStruct SaveStruct;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<int> SaveTArray;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SaveName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTransform SaveTransform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SaveSlot;
	
};
