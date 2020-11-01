// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GGameWidget.h"
#include "GGrid.h"
#include "GObject.h"
#include "GameFramework/PlayerController.h"
#include "GPlayerController.generated.h"

UCLASS()
class GRANGO_API AGPlayerController : public APlayerController
{
	GENERATED_BODY()

    AGPlayerController();

public:

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UI)
    TSubclassOf<UGGameWidget> GameWidget;

    UPROPERTY(BlueprintReadOnly, Category = UI)
    UGGameWidget* GameUMG;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Grid)
    TSubclassOf<class AGGrid> GridClass;

    UPROPERTY()
    UMaterialInterface* GridMaterial;

    UPROPERTY()
    AGGrid* Grid;
    
    UPROPERTY(BlueprintReadOnly, Category = Build)
    bool bIsBuild = false;

    UPROPERTY(BlueprintReadOnly, Category = Build)
    bool bCanBuild = true;

    UPROPERTY(BlueprintReadWrite, Category = Build)
    AGObject* BuildObject;

    UPROPERTY()
    FVector HitLocation;

    UPROPERTY(BlueprintReadOnly, Category = Build)
    bool ValidTileRange;


    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FloorGen)
    int RenderRange = 5;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FloorGen)
    int ChunkLineElement = 10;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FloorGen)
    int VoxelSize = 100;

    UPROPERTY()
    int ChunkSize;
    
    UPROPERTY()
    int ChunkSizeHalf;
    
    UPROPERTY()
    FVector CharacterPosition;
    
    UPROPERTY()
    int ChunkX;
    
    UPROPERTY()
    int ChunkY;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FloorGen)
    TSubclassOf<AActor> Floor;

    UPROPERTY()
    TArray<AActor*> Chunks;
    UPROPERTY()
    TArray<FVector2D> ChunksCords;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Perlin)
    UTexture2D* Perlin;

    FORCEINLINE void ScreenMsg(const FString& Msg)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, *Msg);
    }

    UFUNCTION(BlueprintCallable)
    void BuildSuccess();

    UFUNCTION(BlueprintCallable)
    void ShowGrid();
    
    UFUNCTION()
    void TestPerlin();

protected:
    virtual void Tick(float DeltaSeconds) override;

private:
    virtual void SetupInputComponent() override;

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnMouseLeftClick();

    UFUNCTION()
    void OnTab();

    UFUNCTION()
    void OnEsc();

    UFUNCTION()
    void OnMouseRightClick();

    UFUNCTION()
    void BuildSystem();

    UFUNCTION()
    void ObjectLocationToGrid(FVector Location, float& X, float& Y, float& Z);

    UFUNCTION()
    void UpdateCursorPosition();

    UFUNCTION()
    bool UpdatePosition();

    UFUNCTION()
    void Initialize();

    UFUNCTION()
    void AddChunk();

    UFUNCTION()
    bool CheckRadius(float x, float y);

    UFUNCTION()
    void RemoveChunk();

    
};
