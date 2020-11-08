// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "GGrid.generated.h"

UCLASS()
class GRANGO_API AGGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGGrid();

	
	UPROPERTY()
	UProceduralMeshComponent* LineProceduralMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int NumRows = 50;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int NumColumns = 50;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TileSize = 150;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LineThickness = 5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor LineColor = FColor::White;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LineOpacity = .5f;

	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, *Msg);
	}

	UFUNCTION(BlueprintCallable)
	void LocationToTile(FVector Location, bool& Valid, int& Row, int& Column);

	UFUNCTION(BlueprintCallable)
    void TileToLocation(FVector& Location, int Row, int Column);

	UFUNCTION()
    void InitGrid();

		
	UFUNCTION()
    float GridWidth();
	
	UFUNCTION()
    float GridHeight();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TArray<FVector> LineVertices;
	UPROPERTY()
	TArray<int> LineTriangles;

	
	UFUNCTION()
	void CreateLine(FVector Start, FVector End, float Thinkness);
	
	UFUNCTION()
	UMaterialInterface* CreateMaterialInstance(FLinearColor Color, float Opacity);

	

	UFUNCTION()
	bool TileValid(int Row, int Column);
	

};
