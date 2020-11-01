// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GObject.h"
#include "GNormarTestWall.generated.h"

/**
 * 
 */
UCLASS()
class GRANGO_API AGNormarTestWall : public AGObject
{
	GENERATED_BODY()

public:
	virtual void SetObject() override;

	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, *Msg);
	}
	
};
