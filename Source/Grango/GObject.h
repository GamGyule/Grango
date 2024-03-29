// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GObject.generated.h"


UCLASS()
class GRANGO_API AGObject : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGObject();

    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* GObjectMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UTexture2D* ObjectTexture;

    UPROPERTY()
    UMaterialInterface* Material;

    UPROPERTY()
    bool bIsOverlap = false;

    FORCEINLINE void ScreenMsg(const FString& Msg)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, *Msg);
    }

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void CanBuild(bool bCanBuild);

    UFUNCTION()
    void BuildSuccess();

    
    UFUNCTION()
    virtual void SetObject();

};
