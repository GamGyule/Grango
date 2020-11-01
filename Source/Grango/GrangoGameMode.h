// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GrangoGameMode.generated.h"

UCLASS(minimalapi)
class AGrangoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGrangoGameMode();

	UPROPERTY(BlueprintReadWrite, Category = Settings)
	int RenderRange = 0;

	UPROPERTY(BlueprintReadWrite, Category = Settings)
	int ChunkLineElements = 0;

	UPROPERTY(BlueprintReadWrite, Category = Settings)
	int VoxelSize = 0;

	UPROPERTY(BlueprintReadWrite, Category = Settings)
	int ChunkSize = 0;

	UPROPERTY(BlueprintReadWrite, Category = Settings)
	int ChunkSizeHalf = 0;

	
};



