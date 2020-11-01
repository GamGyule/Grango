// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "GObjectDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FObjectDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ObjectIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ObjectActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ObjectMaterials;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ObjectTools;
	
};

UCLASS()
class GRANGO_API AGObjectDataStruct : public AActor
{
	GENERATED_BODY()

};
