// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GPlayerState.generated.h"


UENUM(BlueprintType)
enum EGPlayerState
{
    Idle UMETA(DisplayName="일반 모드"),
    Building UMETA(DisplayName="건축 모드"),
};